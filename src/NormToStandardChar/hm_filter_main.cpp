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
// 自分自身の定義全体とでも呼べるものを返す。上のaFilterInfo。
// これはこのままコピペで良い
//---------------------------------------------------------------------------------------------------
MACRO_DLL
HIDEMARUFILTERINFO* EnumHidemaruFilter() {
	//---------------------------------------------------------------------------------------------------
	// 秀丸指定のフォーマット。
	// �@ 自身の構造体サイズ
	// �A 呼んでCallBackしてもらう関数名
	// �B メニューに出る日本語名
	// �C メニューに出る英語名
	// �D メニューのショートカットキー
	//---------------------------------------------------------------------------------------------------
	static HIDEMARUFILTERINFO aFilterInfo[14] = {
		{ sizeof(HIDEMARUFILTERINFO), "NormToStandardChar", "標準的な文字への正規化", "Normalization to standard characters", 'N', 0, 0, 0 },
		{ 0, NULL, NULL, NULL, NULL, 0, 0, 0 }
	};
	return aFilterInfo;
}


//---------------------------------------------------------------------------------------------------
// この変換モジュールのバージョン
//---------------------------------------------------------------------------------------------------
MACRO_DLL
DWORD HidemaruFilterGetVersion() {
	double version = 1.03;
	return double_ver_to_hm_filter_ver(version);
}



//---------------------------------------------------------------------------------------------------
// hwnHidemaru   : 秀丸のウィンドウハンドル
//
// pwszIn        : wchar_t* 型で 変換対象となっている文字列が全部渡ってくる。
//                 通常は秀丸エディタで現在編集中の「選択しているテキスト」が渡ってくることになる。
//                 秀丸マクロであれば、「filter関数」を使って第４引数で直接渡すことも可能。
//
// pszParamBuffer: これは自分で自由に自分で決める、パラメータみたいなもの。
//                  複数の使い道がある。
//                 �@まず、秀丸エディタの変換のメニューから選んだ場合は、NULLしか渡ってこない。
//                 �A「秀丸マクロのfilter文やfilter関数」で第３引数に文字列を渡した場合は、そのまま渡ってくる。
//                  この場合は、該当の秀丸マクロのファイル(.mac)の文字コードが不明なため、「英数字」だけが望ましい。
//
//                  通常は「0」と「1」の並びとすることが多い。自分自身で「何番めの文字はどういう意味に使う」と決めることになる。
// 
//                  例えば、秀丸の「Sort変換モジュール」の場合は、
//                  fReverse     = pszParamBuffer[0] == '1' or '0';
//                  fIgnoreSpace = pszParamBuffer[1] == '1' or '0';
//                  fIgnoreCase  = pszParamBuffer[2] == '1' or '0';
//                  fRealNumber  = pszParamBuffer[3] == '1' or '0';
//
//                  を並べて、"1101" などといった形となっており、filter関数などで、そのように渡すことが可能である。
// 
//                 �Bパラメータを読み取るだけではなく、書き込んでおくことも可能。
//                　書き込んでおいたものは、「秀丸マクロのキー操作の記録」をすると、秀丸エディタがこのパラメータを覚えおり、
//                  「キー操作の再生」をすると、そのパラメータをpszParamBufferに渡した状態で、この関数を呼び出すことになる。
// 
//                  このような仕組みがあることで、「マクロのキー操作時の記録」ではダイアログを出して値を選択し、「キー操作再生時」にはダイアログで選んだ値をそのまま使うといったことが可能となっている。
//
// cbParamBuffer : pszParamの秀丸本体側のバッファーサイズ。260。
//---------------------------------------------------------------------------------------------------
wstring wstrNormalizeText = L"";

MACRO_DLL HGLOBAL NormToStandardChar( HWND hwndHidemaru, const wchar_t *pwszIn, char *pszParamBuffer, int cbParamBuffer ) {

	// まず、元のデータをwstring型で確保。
	String^ normalizeText = NormToStandardCharLib::Normalize(gcnew String(pwszIn));

	pin_ptr<const wchar_t> wch = PtrToStringChars(normalizeText);

	// wstring型なので、必ず２倍。
	size_t cb = (normalizeText->Length + 1) * 2;

	HGLOBAL	hmemDst = GlobalAlloc(GMEM_MOVEABLE, cb);
	if (hmemDst == nullptr) {
		return nullptr;
	}

	// ロック
	wchar_t* pwchDest = (wchar_t*)GlobalLock(hmemDst);

	if (pwchDest) {
		// コピペ
		memcpy(pwchDest, wch, cb);
	}
	// アンロック
	GlobalUnlock(pwchDest);
	return pwchDest;
}




