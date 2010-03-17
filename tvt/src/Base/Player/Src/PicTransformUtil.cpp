/*H***************************************************************************
 File            : PicTransformUtil.cpp
 Subsystem       : 
 Function Name(s): CPicTransformUtil
 Author          : YiQiu
 Date            : 2010-2-27  
 Time            : 16:58
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#include "StdAfx.h"
#include "..\Include\PicTransformUtil.h"


CPicTransformUtil::CPicTransformUtil(void)
{
}

CPicTransformUtil::~CPicTransformUtil(void)
{
}

/*
����˵��: ��YUV422ƽ������ת��Ϊ�������
���������ʵ�� : ���CPU֧��MMXָ�������MMX���ʵ�֣�����ʹ��C����ʵ��
����˵��: pDst[OUT]: ת��Ŀ���ַ--��ת��������ݴ��λ��
pSrc[IN] : ת��Դ��ַ----����Ҫת�������ݴ��λ��
nWidth[IN]:ͼ����(PIXEL)
nHeight[IN]:ͼ��߶�(PIXEL)
nPitch[IN]:ÿ������ռ�õ��ֽ����������DDRAW�ı���ʹ���DDSURFACEDESC::lPitch��
�������ͨת�����򴫵�nWidth*2(��Ϊÿ�����������ֽ�)
�޸ļ�¼:
�޸���		�޸�ʱ��		�޸�����
--------------------------------------------------------------------------
zhangzhen	2007/02/02		����YUV422ƽ�����ݴ������Ϊһ������
*/
void CPicTransformUtil::YUV422Pack(
    void *pDst, const void *pSrc,
    unsigned int nWidth, unsigned int nHeight, 
    unsigned int nPitch)
{
    BYTE *pY = (BYTE*)pSrc;
    BYTE *pU = (BYTE*)pSrc + nWidth * nHeight;
    BYTE *pV = (BYTE*)pSrc + nWidth * nHeight * 3 / 2;

    if( !CPUSupport::s_CPUSupport.m_bSupportMMX )//CPU��֧��MMX,ֱ��ת��
    {
        WORD *pwYUV422 = (WORD *)pDst;

        for (unsigned int i = 0; i < nHeight; i++)
        {
            for (unsigned int j = 0; j < nWidth/2; j++)
            {
                *pwYUV422++ = (*pU++) << 8 | *pY++;
                *pwYUV422++ = (*pV++) << 8 | *pY++;
            }
            pwYUV422 += nPitch/2 - /*nPitch*/nWidth;	//Modify By zhangzhen 2007/04/9
        }					
    }
    else//CPU֧��MMX����MMXָ�ת��
    {
        unsigned int count = nWidth*2/32;
        unsigned int diff = nPitch -nWidth*2;
        __asm 
        {
            mov edi, pDst;
            mov esi, pY;
            mov eax, pU;
            mov edx, pV;
            mov ebx, nHeight
LOOP_H:
            mov ecx, count;
LOOP_W:
            movq mm0, [esi + 0];
            movq mm2, [esi + 8];
            movq mm4, [eax];
            movq mm6, [edx];
            movq mm1, mm0;
            movq mm3, mm2;
            movq mm5, mm4;

            PUNPCKLBW MM4, MM6;
            PUNPCKHBW MM5, MM6;

            PUNPCKLBW MM0, MM4;
            PUNPCKHBW MM1, MM4;
            PUNPCKLBW MM2, MM5;
            PUNPCKHBW MM3, MM5;

            movq [edi +  0], mm0;
            movq [edi +  8], mm1;
            movq [edi + 16], mm2;
            movq [edi + 24], mm3;

            add esi, 16;
            add edi, 32;
            add eax, 8;
            add	edx, 8;
            dec ecx;
            jnz LOOP_W;
            add edi,diff;
            dec ebx;
            jnz LOOP_H;
            emms;
        }
    }
}

/*
����˵��: ��YUV420ƽ������ת��ΪYUV422�������
����˵��: pDst[OUT]: ת��Ŀ���ַ--��ת��������ݴ��λ��
pSrc[IN] : ת��Դ��ַ----����Ҫת�������ݴ��λ��
nWidth[IN]:ͼ����(PIXEL)
nHeight[IN]:ͼ��߶�(PIXEL)
nPitch[IN]:ÿ������ռ�õ��ֽ����������DDRAW�ı���ʹ���DDSURFACEDESC::lPitch��
�������ͨת�����򴫵�nWidth*2(��Ϊÿ�����������ֽ�)
YUV420���ݸ�ʽ:ż����(0,2,4...):���������������ع���U��������������һ���������ع���V����
������(1,3,5...):���������������ع���V��������������һ���������ع���U����
�޸ļ�¼:
�޸���		�޸�ʱ��		�޸�����
--------------------------------------------------------------------------
zhangzhen	2007/03/27		<4108-16>
*/
void CPicTransformUtil::YUV420_YUV422Pack(
    void *pDst, const void *pSrc,
    unsigned int nWidth, unsigned int nHeight,
    unsigned int nPitch, INT src420Subtype)
{
    if (src420Subtype == 1)
    {
        int BufWidth;
        if(nWidth == 352)
        {
            BufWidth = 384;
        }
        else if(nWidth == 176)
        {
            BufWidth = 192;
        }
        else
        {
            BufWidth = nWidth;
        }

        BYTE* pY = (BYTE*)pSrc;
        BYTE* pU = pY+BufWidth*nHeight;//2009/02/27
        BYTE* pV = pY+BufWidth*nHeight+BufWidth*nHeight/4;//2009/02/27
        BYTE * pSurf=(BYTE*)pDst;
        for(unsigned int i=0;i<nHeight;i++)//2009/02/27
        {
            for(unsigned int j=0;j<nWidth/2;j++)
            {
                pSurf[j*4] =	pY[j*2];
                pSurf[j*4+1] =	pU[j*2+1];
                pSurf[j*4+2] =	pY[j*2+1];
                pSurf[j*4+3] =	pU[j*2];
            }
            pSurf += nPitch;;
            pY += BufWidth;
            if(i&1)
            {
                pU += BufWidth;
                pV += BufWidth;
            }
        }	
        
        return;
    }
    	
    BYTE *pY = (BYTE*)pSrc;
    BYTE *pU = (BYTE*)pSrc + nWidth * nHeight;
    BYTE *pV = (BYTE*)pU + nWidth * nHeight * 1 / 4;

    if(!CPUSupport::s_CPUSupport.m_bSupportMMX)//CPU��֧��MMX,ֱ��ת��
    {
        BYTE *pYUV422Pack = (BYTE*)pDst;

        for(unsigned int i = 0; i < nHeight; i++)
        {
            for(unsigned int j = 0; j < nWidth/2; j++)
            {
                *pYUV422Pack++ = *pY++;
                *pYUV422Pack++ = *pU++;
                *pYUV422Pack++ = *pY++;
                *pYUV422Pack++ = *pV++;
            }

            pYUV422Pack += nPitch - nWidth*2;

            if(i%2 == 0)	//ż���У�UV�����ֱ�Ҫ��ǰ��һ�У���Ϊ��һ��������Ҫ�����е�UV����
            {
                pU -= nWidth/2;
                pV -= nWidth/2;
            }
        }
    }
    else//CPU֧��MMX����MMXָ�ת��
    {
        unsigned int count = nWidth*2/32;
        unsigned int diff = nPitch -nWidth*2;
        unsigned int line_bytes_uv = nWidth/2;
        __asm 
        {
            mov edi, pDst;
            mov esi, pY;
            mov eax, pU;
            mov edx, pV;
            mov ebx, nHeight
LOOP_H:
            mov ecx, count;
LOOP_W:
            movq mm0, [esi + 0];
            movq mm2, [esi + 8];
            movq mm4, [eax];
            movq mm6, [edx];
            movq mm1, mm0;
            movq mm3, mm2;
            movq mm5, mm4;

            PUNPCKLBW MM4, MM6;
            PUNPCKHBW MM5, MM6;

            PUNPCKLBW MM0, MM4;
            PUNPCKHBW MM1, MM4;
            PUNPCKLBW MM2, MM5;
            PUNPCKHBW MM3, MM5;

            movq [edi +  0], mm0;
            movq [edi +  8], mm1;
            movq [edi + 16], mm2;
            movq [edi + 24], mm3;

            add esi, 16;
            add edi, 32;
            add eax, 8;
            add	edx, 8;
            dec ecx;
            jnz LOOP_W;
            add edi,diff;
            test ebx, 0x1
            jnz	YU_NEXT_LINE		;����������ǰ��һż���й���UV����
            sub	eax, line_bytes_uv
            sub edx, line_bytes_uv
YU_NEXT_LINE:
            dec ebx;
            jnz LOOP_H;
            emms;
        }
    }
   
}










// End of file



