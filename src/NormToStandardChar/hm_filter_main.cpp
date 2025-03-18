/*
 * Copyright (c) 2025 Akitsugu Komiyama
 * under the MIT License
 */
#include <windows.h>

#include "hm_filter.h"

#include <vcclr.h>


#include "normalize.h"

using namespace std;



//---------------------------------------------------------------------------------------------------
// �������g�̒�`�S�̂Ƃł��Ăׂ���̂�Ԃ��B���aFilterInfo�B
// ����͂��̂܂܃R�s�y�ŗǂ�
//---------------------------------------------------------------------------------------------------
MACRO_DLL
HIDEMARUFILTERINFO* EnumHidemaruFilter() {
	//---------------------------------------------------------------------------------------------------
	// �G�ێw��̃t�H�[�}�b�g�B
	// �@ ���g�̍\���̃T�C�Y
	// �A �Ă��CallBack���Ă��炤�֐���
	// �B ���j���[�ɏo����{�ꖼ
	// �C ���j���[�ɏo��p�ꖼ
	// �D ���j���[�̃V���[�g�J�b�g�L�[
	//---------------------------------------------------------------------------------------------------
	static HIDEMARUFILTERINFO aFilterInfo[14] = {
		{ sizeof(HIDEMARUFILTERINFO), "NormToStandardChar", "�W���I�ȕ����ւ̐��K��", "Normalization to standard characters", 'N', 0, 0, 0 },
		{ 0, NULL, NULL, NULL, NULL, 0, 0, 0 }
	};
	return aFilterInfo;
}


//---------------------------------------------------------------------------------------------------
// ���̕ϊ����W���[���̃o�[�W����
//---------------------------------------------------------------------------------------------------
MACRO_DLL
DWORD HidemaruFilterGetVersion() {
	double version = 1.02;
	return double_ver_to_hm_filter_ver(version);
}



//---------------------------------------------------------------------------------------------------
// hwnHidemaru   : �G�ۂ̃E�B���h�E�n���h��
//
// pwszIn        : wchar_t* �^�� �ϊ��ΏۂƂȂ��Ă��镶���񂪑S���n���Ă���B
//                 �ʏ�͏G�ۃG�f�B�^�Ō��ݕҏW���́u�I�����Ă���e�L�X�g�v���n���Ă��邱�ƂɂȂ�B
//                 �G�ۃ}�N���ł���΁A�ufilter�֐��v���g���đ�S�����Œ��ړn�����Ƃ��\�B
//
// pszParamBuffer: ����͎����Ŏ��R�Ɏ����Ō��߂�A�p�����[�^�݂����Ȃ��́B
//                  �����̎g����������B
//                 �@�܂��A�G�ۃG�f�B�^�̕ϊ��̃��j���[����I�񂾏ꍇ�́ANULL�����n���Ă��Ȃ��B
//                 �A�u�G�ۃ}�N����filter����filter�֐��v�ő�R�����ɕ������n�����ꍇ�́A���̂܂ܓn���Ă���B
//                  ���̏ꍇ�́A�Y���̏G�ۃ}�N���̃t�@�C��(.mac)�̕����R�[�h���s���Ȃ��߁A�u�p�����v�������]�܂����B
//
//                  �ʏ�́u0�v�Ɓu1�v�̕��тƂ��邱�Ƃ������B�������g�Łu���Ԃ߂̕����͂ǂ������Ӗ��Ɏg���v�ƌ��߂邱�ƂɂȂ�B
// 
//                  �Ⴆ�΁A�G�ۂ́uSort�ϊ����W���[���v�̏ꍇ�́A
//                  fReverse     = pszParamBuffer[0] == '1' or '0';
//                  fIgnoreSpace = pszParamBuffer[1] == '1' or '0';
//                  fIgnoreCase  = pszParamBuffer[2] == '1' or '0';
//                  fRealNumber  = pszParamBuffer[3] == '1' or '0';
//
//                  ����ׂāA"1101" �ȂǂƂ������`�ƂȂ��Ă���Afilter�֐��ȂǂŁA���̂悤�ɓn�����Ƃ��\�ł���B
// 
//                 �B�p�����[�^��ǂݎ�邾���ł͂Ȃ��A��������ł������Ƃ��\�B
//                �@��������ł��������̂́A�u�G�ۃ}�N���̃L�[����̋L�^�v������ƁA�G�ۃG�f�B�^�����̃p�����[�^���o������A
//                  �u�L�[����̍Đ��v������ƁA���̃p�����[�^��pszParamBuffer�ɓn������ԂŁA���̊֐����Ăяo�����ƂɂȂ�B
// 
//                  ���̂悤�Ȏd�g�݂����邱�ƂŁA�u�}�N���̃L�[���쎞�̋L�^�v�ł̓_�C�A���O���o���Ēl��I�����A�u�L�[����Đ����v�ɂ̓_�C�A���O�őI�񂾒l�����̂܂܎g���Ƃ��������Ƃ��\�ƂȂ��Ă���B
//
// cbParamBuffer : pszParam�̏G�ۖ{�̑��̃o�b�t�@�[�T�C�Y�B260�B
//---------------------------------------------------------------------------------------------------
wstring wstrNormalizeText = L"";

MACRO_DLL HGLOBAL NormToStandardChar( HWND hwndHidemaru, const wchar_t *pwszIn, char *pszParamBuffer, int cbParamBuffer ) {

	// �܂��A���̃f�[�^��wstring�^�Ŋm�ہB
	String^ normalizeText = NormToStandardCharLib::Normalize(gcnew String(pwszIn));

	pin_ptr<const wchar_t> wch = PtrToStringChars(normalizeText);

	// wstring�^�Ȃ̂ŁA�K���Q�{�B
	size_t cb = (normalizeText->Length + 1) * 2;

	HGLOBAL	hmemDst = GlobalAlloc(GMEM_MOVEABLE, cb);
	if (hmemDst == nullptr) {
		return nullptr;
	}

	// ���b�N
	wchar_t* pwchDest = (wchar_t*)GlobalLock(hmemDst);
	// �R�s�y
	wcscpy(pwchDest, wch);
	// �A�����b�N
	GlobalUnlock(pwchDest);
	return pwchDest;
}




