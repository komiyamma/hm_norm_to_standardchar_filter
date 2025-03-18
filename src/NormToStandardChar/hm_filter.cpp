/*
 * Copyright (c) 2025 Akitsugu Komiyama
 * under the MIT License
 */

#define _CRT_SECURE_NO_WARNINGS
#include "hm_filter.h"

using namespace std;



//---------------------------------------------------------------------------------------------------
// ���ʂ̃o�[�W�������l �� �G�ەϊ����W���[���^�C�v�̃o�[�W�����ɕϊ�����
//---------------------------------------------------------------------------------------------------
int double_ver_to_hm_filter_ver(const double ver) {
	// �}�C�i�[�o�[�W�����������o���āc
	double minor = ver - (int)ver;

	// ���W���[�o�[�W�����������o����
	int major = int(ver - minor);

	// �G�ۂ��~����`�ւƕϊ�
	return (major << 16) + int(100 * minor);
}
