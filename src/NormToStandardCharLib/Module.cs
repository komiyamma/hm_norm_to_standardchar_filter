using System.Text;
using System.Threading.Tasks;
using System;

public class NormToStandardCharLib
{
    public static string Normalize(string text)
    {
        try
        {
            if (string.IsNullOrEmpty(text))
            {
                return text;
            }

            var lines = text.Split('\n');

            Parallel.For(0, lines.Length, i =>
            {
                lines[i] = NormalizeLine(lines[i]);
            });

            return String.Join("\n", lines);
        }
        catch (Exception ex)
        {
            return ex.Message;
        }
    }

    private static Encoding encoder = Encoding.GetEncoding("Shift_JIS");
    private static int CanConvertSJIS(string text)
    {
        try
        {
            var encodedBytes = encoder.GetBytes(text);
            var decodedText = encoder.GetString(encodedBytes);
            if ( text == decodedText ) {
                return 1;
            } else {
                return 0;
            }
        }
        catch (Exception e)
        {
            return 0;
        }
    }

    private static string NormalizeLine(string text)
    {
        if (CanConvertSJIS(text) == 1)
        {
            return text;
        }

        var walker = System.Globalization.StringInfo.GetTextElementEnumerator(text);

        var dstText = "";
        while (walker.MoveNext())
        {
            var chr = walker.GetTextElement();
            dstText += NormalizeCharacter(chr);
        }
        return dstText;
    }

    private static string NormalizeCharacter(string text)
    {
        if (CanConvertSJIS(text) == 1)
        {
            return text;
        }

        var textNFC = text.Normalize(System.Text.NormalizationForm.FormC);
        if (text != textNFC)
        {
            return textNFC;
        }

        var textNFKC = text.Normalize(System.Text.NormalizationForm.FormKC);
        if (textNFKC.Length <= text.Length)
        {
            return textNFKC;
        }

        return text;
    }
}
