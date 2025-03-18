/*
 * Copyright (c) 2025 Akitsugu Komiyama
 * under the MIT License
 */

#define _CRT_SECURE_NO_WARNINGS
#include "hm_filter.h"

using namespace std;



//---------------------------------------------------------------------------------------------------
// 普通のバージョン数値 → 秀丸変換モジュールタイプのバージョンに変換する
//---------------------------------------------------------------------------------------------------
int double_ver_to_hm_filter_ver(const double ver) {
	// マイナーバージョン部分を出して…
	double minor = ver - (int)ver;

	// メジャーバージョン部分を出して
	int major = int(ver - minor);

	// 秀丸が欲する形へと変換
	return (major << 16) + int(100 * minor);
}
