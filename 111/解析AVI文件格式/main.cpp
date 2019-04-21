#include <iostream>
#include <fstream>
using namespace std;
#pragma pack(1)				//�趨Ϊ1�ֽڶ���
#define AVIF_HASINDEX  0x00000010       //Index at end of file?
typedef unsigned short BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef unsigned long FOURCC;
// MainAVIHeader
typedef struct {
	DWORD dwMicroSecPerFrame;		//��ʾÿ�������ʱ��ns������avi����ʾ����
	DWORD dwMaxBytesPerSec;			//�������ݴ�����
	DWORD dwPaddingGranularity;		//��¼��ĳ�����Ϊ��ֵ�ı�����ͨ����2048
	DWORD dwFlages;				//AVI�ļ����������ԣ����Ƿ���������飬����Ƶ�����Ƿ񽻲�洢
	DWORD dwTotalFrame;			//�ļ��е�������
	DWORD dwInitialFrames;			//˵���ڿ�ʼ����ǰ��Ҫ������
	DWORD dwStreams;				//�ļ��а���������������
	DWORD dwSuggestedBufferSize;	       //����ʹ�õĻ������Ĵ�С��
										   //ͨ��Ϊ�洢һ��ͼ���Լ�ͬ����������Ҫ������֮��
	DWORD dwWidth;				//ͼ���
	DWORD dwHeight;				//ͼ���
	DWORD dwReserved[4];			//����ֵ
}MainAVIHeader;
typedef struct {
	WORD wLeft;
	WORD wTop;
	WORD wRight;
	WORD wBottom;
}RECT;
// strh
typedef struct {
	FOURCC fccType;			//4�ֽڣ���ʾ������������ vids ��ʾ��Ƶ������
							//auds ��Ƶ������
	FOURCC fccHandler;		//4�ֽ� ����ʾ��������ѹ���������������
	DWORD dwFlags;			//����������
	WORD wPriority;			//���������Ĳ������ȼ�
	WORD wLanguage;			//��Ƶ�����Դ���
	DWORD dwInitalFrames;	        //˵���ڿ�ʼ����ǰ��Ҫ������
	DWORD dwScale;			//����������Ƶÿ��Ĵ�С������Ƶ�Ĳ�����С
	DWORD dwRate;			//dwScale /dwRate = ÿ��Ĳ�����
	DWORD dwStart;			//��������ʼ���ŵ�λ�ã���dwScaleΪ��λ
	DWORD dwLength;			//������������������dwScaleΪ��λ
	DWORD dwSuggestedBufferSize;	//���黺�����Ĵ�С
	DWORD dwQuality;			//��ѹ������������ֵԽ������Խ��
	DWORD dwSampleSize;		//��Ƶ�Ĳ�����С
	RECT rcFrame;			//��Ƶͼ����ռ�ľ���
}AVIStreamHeader;
// strf:strh�ӿ�����Ƶ������
typedef struct {
	BYTE rgbBlue;		// ��ɫ����
	BYTE rgbGreen;		// ��ɫ����
	BYTE rgbRed;		// ��ɫ����
	BYTE rgbReserved;	// �����ֽڣ�����Alphaͨ������ԣ�
} RGBQUAD;
typedef struct {
	DWORD dwSize;
	DWORD dwWidth;
	DWORD dwHeight;
	WORD wPlanes;
	WORD wBitCount;
	DWORD dwCompression;
	DWORD dwSizeImage;
	DWORD dwXPelsPerMeter;
	DWORD dwYPelsPerMeter;
	DWORD dwClrUsed;
	DWORD dwClrImportant;
}BITMAPINFOHEADER;
// strf:strh�ӿ�����Ƶ������
typedef struct
{
	WORD wFormatTag;
	WORD wChannels;		//������
	DWORD dwSamplesPerSec;	//������
	DWORD dwAvgBytesPerSec; //WAVE������ÿ���������
	WORD wBlockAlign;	//���ݿ�Ķ����־
	WORD wBitsPerSample;    //ÿ�β�����������
	WORD wSize;		//�˽ṹ�Ĵ�С
}WAVEFORMAT;
typedef struct {
	char chunk_id[4];
	DWORD is_key;
	DWORD pos;
	DWORD size;
}AVI_IDX;
void aviListInfo(char *pInfoBuf, int *readPos)
{
	char buf[5];
	DWORD dwValue = 0;
	int flag = 0;
	// list
	memset(buf, 0, 5);
	memcpy(buf, pInfoBuf + *readPos, 4);
	*readPos += 4;
	cout << " list: " << buf << endl;
	// list len
	dwValue = 0;
	memcpy((char *)&dwValue, pInfoBuf + *readPos, 4);
	*readPos += 4;
	cout << " list len: " << dwValue << endl;
	// strl
	memset(buf, 0, 5);
	memcpy(buf, pInfoBuf + *readPos, 4);
	*readPos += 4;
	cout << " strl: " << buf << endl;

	// strh
	memset(buf, 0, 5);
	memcpy(buf, pInfoBuf + *readPos, 4);
	*readPos += 4;
	cout << " strh: " << buf << endl;
	// strh len
	dwValue = 0;
	memcpy((char *)&dwValue, pInfoBuf + *readPos, 4);
	*readPos += 4;
	cout << " strh len: " << dwValue << endl;
	// strh info
	AVIStreamHeader streamHeader;
	memset(&streamHeader, 0, sizeof(AVIStreamHeader));
	memcpy((char *)&streamHeader, pInfoBuf + *readPos, sizeof(AVIStreamHeader));
	*readPos += sizeof(AVIStreamHeader);
	{
		cout << endl;
		cout << " AVIStreamHeader len: " << sizeof(AVIStreamHeader) << endl;

		memset(buf, 0, 5);
		memcpy(buf, (char *)&streamHeader.fccType, 4);
		cout << " fccType: " << buf << endl;
		string fccType(buf);
		if (0 == fccType.compare(0, 4, "vids")) {  // �ļ�����У��
			flag = 0;
		}
		else {
			flag = 1;
		}
		memset(buf, 0, 5);
		memcpy(buf, (char *)&streamHeader.fccHandler, 4);
		cout << " fccHandler: " << buf << endl;

		cout << " Flags: " << streamHeader.dwFlags << endl;
		cout << " Priority: " << streamHeader.dwQuality << endl;
		cout << " Language: " << streamHeader.wLanguage << endl;
		cout << " InitalFrames: " << streamHeader.dwInitalFrames << endl;
		cout << " Scale: " << streamHeader.dwScale << endl;
		cout << " Rate: " << streamHeader.dwRate << endl;
		cout << " Start: " << streamHeader.dwStart << endl;
		cout << " Length: " << streamHeader.dwLength << endl;
		cout << " SuggestedBufferSize: " << streamHeader.dwSuggestedBufferSize << endl;
		cout << " Quality: " << streamHeader.dwQuality << endl;
		cout << " SampleSize: " << streamHeader.dwSampleSize << endl;
		cout << " left: " << streamHeader.rcFrame.wLeft << endl;
		cout << " top: " << streamHeader.rcFrame.wTop << endl;
		cout << " right: " << streamHeader.rcFrame.wRight << endl;
		cout << " bottom: " << streamHeader.rcFrame.wBottom << endl << endl;
	}
	// strf
	memset(buf, 0, 5);
	memcpy(buf, pInfoBuf + *readPos, 4);
	*readPos += 4;
	cout << " strf: " << buf << endl;
	// strf len
	dwValue = 0;
	memcpy((char *)&dwValue, pInfoBuf + *readPos, 4);
	*readPos += 4;
	cout << " strf len: " << dwValue << endl;
	if (0 == flag) // ��Ƶ��Ϣ
	{
		// strf info:��Ƶ��
		BITMAPINFOHEADER bmiHeader;
		memset(&bmiHeader, 0, sizeof(BITMAPINFOHEADER));
		memcpy((char *)&bmiHeader, pInfoBuf + *readPos, sizeof(BITMAPINFOHEADER));
		*readPos += sizeof(BITMAPINFOHEADER);
		{
			cout << endl;
			cout << " BITMAPINFOHEADER len: " << sizeof(BITMAPINFOHEADER) << endl;
			cout << " Size: " << bmiHeader.dwSize << endl;
			cout << " Width: " << bmiHeader.dwWidth << endl;
			cout << " Height: " << bmiHeader.dwHeight << endl;
			cout << " Planes: " << bmiHeader.wPlanes << endl;
			cout << " BitCount: " << bmiHeader.wBitCount << endl;
			cout << " Compression: " << bmiHeader.dwCompression << endl;
			cout << " SizeImage: " << bmiHeader.dwSizeImage << endl;
			cout << " XPelsPerMeter: " << bmiHeader.dwXPelsPerMeter << endl;
			cout << " YPelsPerMeter: " << bmiHeader.dwYPelsPerMeter << endl;
			cout << " ClrUsed: " << bmiHeader.dwClrUsed << endl;
			cout << " ClrImportant: " << bmiHeader.dwClrImportant << endl;
		}
	}
	else
	{
		WAVEFORMAT waveFormat;
		memset(&waveFormat, 0, sizeof(WAVEFORMAT));
		memcpy((char *)&waveFormat, pInfoBuf + *readPos, sizeof(WAVEFORMAT));
		*readPos += sizeof(WAVEFORMAT);
		{
			cout << endl;
			cout << " WAVEFORMAT len: " << sizeof(WAVEFORMAT) << endl;
			cout << " FormatTag: " << waveFormat.wFormatTag << endl;
			cout << " Channels: " << waveFormat.wChannels << endl;
			cout << " SamplesPerSec: " << waveFormat.dwSamplesPerSec << endl;
			cout << " AvgBytesPerSec: " << waveFormat.dwAvgBytesPerSec << endl;
			cout << " BlockAlign: " << waveFormat.wBlockAlign << endl;
			cout << " BitsPerSample: " << waveFormat.wBitsPerSample << endl;
			cout << " Size: " << waveFormat.wSize << endl;
		}
		*readPos += waveFormat.wSize;
	}
}
int main(int argc, char* argv[])
{
	//char fileName[32] = { 0 };
	char *fileName="traffic.avi";
	char buf[5];
	DWORD dwValue = 0;
	DWORD fileLen = 0;
	DWORD frameCount = 0;
	int hdrlListLen = 0;
	char *pInfo = NULL;
	int readPos = 0;
	DWORD moviPos = 0;
	//cout << "Pleade input file name:";
	//cin >> fileName;
	cout << " File name:" << fileName << endl;
	ifstream aviFile;
	streampos   pos;

	// open
	aviFile.open(fileName, ios::in | ios::binary);
	if (!aviFile)
	{
		cout << fileName << " open error!" << endl;
		return -1;
	}
	// RIFF-4�ֽ�
	memset(buf, 0, 5);
	aviFile.read(buf, 4);
	cout << " riffHead: " << buf << endl;
	// file Len-4�ֽڣ��ļ��ܳ���-8 
	aviFile.read((char *)&dwValue, 4);
	cout << " file len: " << dwValue << endl;
	// file real len 
	pos = aviFile.tellg();     // save current position   
	aviFile.seekg(0, ios::end);
	fileLen = aviFile.tellg();
	aviFile.seekg(pos);			   // restore saved position 

	if (fileLen != (dwValue + 8))       // �ļ���СУ��
	{
		cout << " File is damaged!" << endl;
		return -1;
	}
	// file Type-4�ֽ�
	memset(buf, 0, 5);
	aviFile.read(buf, 4);
	cout << " file type: " << buf << endl;
	string aviFormat(buf);
	if (0 != aviFormat.compare(0, 3, "AVI"))  // �ļ�����У��
	{
		cout << " Isn't AVI file!" << endl;
		return -1;
	}
	// hdrl list-4�ֽ�
	memset(buf, 0, 5);
	aviFile.read(buf, 4);
	cout << " list: " << buf << endl;

	// list Len-4�ֽ�
	dwValue = 0;
	aviFile.read((char *)&dwValue, 4);
	cout << " list len: " << dwValue << endl;
	hdrlListLen = dwValue;
	pInfo = new char[hdrlListLen + 1];
	memset(pInfo, 0, hdrlListLen + 1);
	aviFile.read(pInfo, dwValue);
	{
		readPos = 0;

		// list Type-4�ֽ�
		memset(buf, 0, 5);
		memcpy(buf, pInfo + readPos, 4);
		readPos += 4;
		cout << " list type: " << buf << endl;
		// avih-4�ֽ�:
		memset(buf, 0, 5);
		memcpy(buf, pInfo + readPos, 4);
		readPos += 4;
		cout << " avih: " << buf << endl;

		// avih Len-4�ֽ�
		dwValue = 0;
		memcpy((char *)&dwValue, pInfo + readPos, 4);
		readPos += 4;
		cout << " avih len: " << dwValue << endl;
		// avi header
		MainAVIHeader mainAVIHeader;
		memset(&mainAVIHeader, 0, sizeof(MainAVIHeader));
		memcpy((char *)&mainAVIHeader, pInfo + readPos, dwValue);
		readPos += dwValue;
		{
			cout << endl;
			cout << " MainAVIHeader len: " << sizeof(MainAVIHeader) << endl;
			cout << " MicroSecPerFrame: " << mainAVIHeader.dwMicroSecPerFrame << endl;
			cout << " MaxBytesPerSec: " << mainAVIHeader.dwMaxBytesPerSec << endl;
			cout << " PaddingGranularity: " << mainAVIHeader.dwPaddingGranularity << endl;
			cout << " Flages: " << mainAVIHeader.dwFlages << endl;
			cout << " TotalFrame: " << mainAVIHeader.dwTotalFrame << endl;
			frameCount = mainAVIHeader.dwTotalFrame;
			cout << " InitialFrames: " << mainAVIHeader.dwInitialFrames << endl;
			cout << " Streams: " << mainAVIHeader.dwStreams << endl;
			cout << " SuggestedBufferSize: " << mainAVIHeader.dwSuggestedBufferSize << endl;
			cout << " Width: " << mainAVIHeader.dwWidth << endl;
			cout << " Height: " << mainAVIHeader.dwHeight << endl;
		}
		if (!mainAVIHeader.dwFlages & AVIF_HASINDEX)
		{
			cout << " Hasn't find idx info!" << endl;
			return -1;
		}
		// list1
		cout << endl << " ======List1======" << endl;
		aviListInfo(pInfo, &readPos);

		// list 2
		if (mainAVIHeader.dwStreams>1)
		{
			int i;
			for (i = readPos; i<hdrlListLen;)
			{
				memset(buf, 0, 5);
				memcpy(buf, pInfo + readPos, 4);
				cout << " buf: " << buf << endl;

				string listFlag(buf);
				if (0 == listFlag.compare(0, 4, "LIST")) break;
				readPos += 4;
				i = readPos;
			}
			// list2
			cout << endl << " ======List2======" << endl;
			aviListInfo(pInfo, &readPos);
		}
		cout << " readPos=" << readPos << endl;
	}
	delete[]pInfo;
	pInfo = NULL;
	// find list
	{
		int i;
		string listFlag;
		for (i = readPos + 8; i<fileLen;)
		{
			memset(buf, 0, 5);
			aviFile.read(buf, 4);
			cout << " buf: " << buf << endl;

			listFlag = buf;
			if (0 == listFlag.compare(0, 4, "LIST"))
			{
				// len
				dwValue = 0;
				aviFile.read((char *)&dwValue, 4);
				i += 4;
				cout << " len: " << dwValue << endl;
				// type
				memset(buf, 0, 5);
				aviFile.read(buf, 4);
				i += 4;
				cout << " type: " << buf << endl;

				listFlag = buf;
				aviFile.seekg(dwValue - 4, ios::cur);
				if (0 == listFlag.compare(0, 4, "movi"))
				{
					moviPos = aviFile.tellg();
					moviPos = moviPos - dwValue + 4;
					break;
				}
			}
			else if (0 == listFlag.compare(0, 4, "JUNK"))
			{
				// len
				dwValue = 0;
				aviFile.read((char *)&dwValue, 4);
				cout << " len: " << dwValue << endl;
				aviFile.seekg(dwValue, ios::cur);
			}
			i += 4;
		}
	}
	if (0 == moviPos) return -1;
	cout << endl << " ========= idxl info ===========" << endl;
	// idxl
	memset(buf, 0, 5);
	aviFile.read(buf, 4);
	cout << " idxl: " << buf << endl;
	// avih Len-4�ֽ�
	dwValue = 0;
	aviFile.read((char *)&dwValue, 4);
	cout << " idxl len: " << dwValue << endl;
	// avi file index
	AVI_IDX *pIndx = new AVI_IDX[frameCount];
	char *pszBuf = new char[1024 * 1024];
	DWORD nBufSize = 1024 * 1024;
	// open video file
	ofstream videoFile;
	videoFile.open("test.video", ios::out | ios::binary);
	if (dwValue >= frameCount * sizeof(AVI_IDX))
		dwValue = frameCount * sizeof(AVI_IDX);
	aviFile.read((char *)pIndx, dwValue);
	{
		int idx;
		for (idx = 0; idx<frameCount; ++idx)
			//for(idx=0;idx<50;++idx)
		{
			//memset(buf,0,5);
			//memcpy(buf,pIndx[idx].chunk_id,4);
			// cout<< endl<< "  id:" <<  buf << endl;
			// cout<< "  key:" <<  pIndx[idx].is_key << endl;
			// cout<< "  pos:" <<  pIndx[idx].pos << endl;
			// cout<< "  size:" <<  pIndx[idx].size << endl;
			aviFile.seekg(moviPos + pIndx[idx].pos, ios::beg);
			memset(pszBuf, 0, nBufSize);
			aviFile.read(pszBuf, pIndx[idx].size);
			if (!strncmp(pIndx[idx].chunk_id, "00dc", 4));
			videoFile.write(pszBuf, pIndx[idx].size);
		}
	}
	delete[]pIndx;
	pIndx = NULL;
	delete[]pszBuf;
	pszBuf = NULL;
	videoFile.close();
	aviFile.close();
	return 0;
}
