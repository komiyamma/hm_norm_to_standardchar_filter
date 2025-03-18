
using namespace System::Text;
using namespace System::Threading::Tasks;
using namespace System;

public ref class NormToStandardCharLib
{
public:
	static String^ Normalize(String^ text);

private:
	static Encoding^ encoder = Encoding::GetEncoding("Shift_JIS");
	static int CanConvertSJIS(String^ text);

	static String^ NormalizeLine(String^ text);

	static String^ NormalizeCharacter(String^ text);
};

