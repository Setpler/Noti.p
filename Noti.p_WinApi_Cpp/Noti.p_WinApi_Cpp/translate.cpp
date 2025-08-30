#include <windows.h>
#include "Functions.h"

void Translate() {
    LANGID langId = GetUserDefaultUILanguage();
    WORD primaryLang = PRIMARYLANGID(langId);
    
    switch (primaryLang)
    {
    case LANG_ENGLISH:
        error_word = L"Error";
        cannot_open_file_word = L"Cannot open file";
        cannot_save_file_word = L"Cannot save file";
        file_is_big_or_error_size_word = L"File is too big or error size";
        error_convert_text_word = L"Error converting text";
        error_regist_class_error_word = L"Error registering class";
        count_symbols_txt_pole_word = L"Count symbols in text field: ";
        select_all_word = L"Select All";
        copy_word = L"Copy";
        cut_word = L"Cut";
        redo_word = L"Redo";
        undo_word = L"Undo";
        paste_word = L"Paste";
        save_as_word = L"Save As";
        save_word = L"Save";
        open_word = L"Open";
        all_files_word = L"All Files";
        txt_files_word = L"Text Files (*.txt)";
        more_word = L"MORE";
        clear_word = L"Clear";
        to_cipher_word = L"Crypt"; // Або "Cipher", якщо потрібно саме "to cipher"
		FILE_word = L"FILE";
		parametres_word = L"Settings";
		forward_word = L"INSERT"; // Якщо потрібно
        to_encrypt_word = L"Encrypt"; // Якщо потрібно
        to_decipher_word = L"Decrypt"; // Якщо потрібно
        method_word = L"Method";
        break;
    case LANG_UKRAINIAN:
        error_word = L"Помилка";
        cannot_open_file_word = L"Не вдалося відкрити файл";
        cannot_save_file_word = L"Не вдалося зберегти файл";
        file_is_big_or_error_size_word = L"Файл занадто великий або помилка розміру";
        error_convert_text_word = L"Помилка конвертації тексту";
        error_regist_class_error_word = L"Помилка реєстрації класу";
        count_symbols_txt_pole_word = L"Кількість символів у полі тексту: ";
        select_all_word = L"Вибрати все";
        copy_word = L"Копіювати";
        cut_word = L"Вирізати";
        redo_word = L"Вгору";
        undo_word = L"Вниз";
        paste_word = L"Вставити";
        save_as_word = L"Зберегти як";
        save_word = L"Зберегти";
        open_word = L"Відкрити";
        all_files_word = L"Усі файли";
        txt_files_word = L"Текстові файли (*.txt)";
        more_word = L"БІЛЬШЕ";
        clear_word = L"Очистити";
        to_cipher_word = L"Шифрування"; // Або "Шифрувати"
		FILE_word = L"ФАЙЛ";
		parametres_word = L"Налаштування";
		forward_word = L"ВСТАВКА"; // Якщо потрібно
        to_encrypt_word = L"Зашифрувати"; // Якщо потрібно
        to_decipher_word = L"Розшифрувати"; // Якщо потрібно
        method_word = L"Метод"; // Якщо потрібно
        break;
    case LANG_CHINESE:
        error_word = L"错误";
        cannot_open_file_word = L"无法打开文件";
        cannot_save_file_word = L"无法保存文件";
        file_is_big_or_error_size_word = L"文件太大或大小错误";
        error_convert_text_word = L"文本转换错误";
        error_regist_class_error_word = L"注册类错误";
        count_symbols_txt_pole_word = L"文本字段中的字符数: ";
        select_all_word = L"全选";
        copy_word = L"复制";
        cut_word = L"剪切";
        redo_word = L"上";
        undo_word = L"下";
        paste_word = L"粘贴";
        save_as_word = L"另存为";
        save_word = L"保存";
        open_word = L"打开";
        all_files_word = L"所有文件";
        txt_files_word = L"文本文件 (*.txt)";
        more_word = L"更多";
        clear_word = L"清除";
        to_cipher_word = L"加密"; // "Jiāmì" (шифрувати)
		FILE_word = L"文件"; // "Wénjiàn" (файл)
		parametres_word = L"设置"; // "Shèzhì" (налаштування)
		forward_word = L"插入"; // "Chārù" (вставка)
        to_encrypt_word = L"加密"; // "Jiāmì" (шифрувати)
        to_decipher_word = L"解密"; // "Jiěmì" (розшифрувати)
        method_word = L"方法"; // "Fāngfǎ" (метод)
        break;
    case LANG_JAPANESE:
        error_word = L"エラー";
        cannot_open_file_word = L"ファイルを開けません";
        cannot_save_file_word = L"ファイルを保存できません";
        file_is_big_or_error_size_word = L"ファイルが大きすぎるかサイズエラー";
        error_convert_text_word = L"テキスト変換エラー";
        error_regist_class_error_word = L"クラス登録エラー";
        count_symbols_txt_pole_word = L"テキストフィールドの文字数: ";
        select_all_word = L"すべて選択";
        copy_word = L"コピー";
        cut_word = L"切り取り";
        redo_word = L"上";
        undo_word = L"榮耀歸於烏克蘭！不幸的是，我沒有找到這個詞的翻譯:( 也許是 '下' 或 '撤消'";
        paste_word = L"貼り付け";
        save_as_word = L"名前を付けて保存";
        save_word = L"保存";
        open_word = L"開く";
        all_files_word = L"すべてのファイル";
        txt_files_word = L"テキストファイル (*.txt)";
        more_word = L"もっと";
        clear_word = L"クリア";
        to_cipher_word = L"暗号化"; // "Angōka" (шифрувати)
		FILE_word = L"ファイル"; // "Fairu" (файл)  
		parametres_word = L"設定"; // "Settei" (налаштування)
		forward_word = L"挿入"; // "Sōnyū" (вставка)
        to_encrypt_word = L"暗号化"; // "Angōka" (шифрувати)
        to_decipher_word = L"復号化"; // "Fukugōka" (розшифрувати)
        method_word = L"方法"; // "Hōhō" (метод)
        break;
    case LANG_AZERBAIJANI:
        error_word = L"Səhv";
        cannot_open_file_word = L"Fayl açıla bilmir";
        cannot_save_file_word = L"Fayl saxlanıla bilmir";
        file_is_big_or_error_size_word = L"Fayl çox böyükdür və ya ölçü xətası";
        error_convert_text_word = L"Mətn çevirmə xətası";
        error_regist_class_error_word = L"Sinif qeydiyyat xətası";
        count_symbols_txt_pole_word = L"Mətn sahəsində simvolların sayı: ";
        select_all_word = L"Hamısını seç";
        copy_word = L"Kopyala";
        cut_word = L"Kəs";
        redo_word = L"Yuxarı";
        undo_word = L"Aşağı";
        paste_word = L"Yapışdır";
        save_as_word = L"Fərqli saxla";
        save_word = L"Saxla";
        open_word = L"Aç";
        all_files_word = L"Bütün fayllar";
        txt_files_word = L"Mətn faylları (*.txt)";
        more_word = L"DAHA ÇOX";
        clear_word = L"Təmizlə";
        to_cipher_word = L"Şifrələ"; // Або "Kodla"
		FILE_word = L"FAYL"; // "Fayl" (файл)
		parametres_word = L"Ayarlar"; // "Ayarlar" (налаштування)
		forward_word = L"ƏLAVə ET"; // "Əlavə et" (вставка)
        to_encrypt_word = L"Şifrələmək"; // "Şifrələmək" (шифрувати)
        to_decipher_word = L"Şifrəni açmaq"; // "Şifrəni açmaq" (розшифрувати)
        method_word = L"Metod"; // "Metod" (метод)
        break;
    case LANG_KAZAK:
        error_word = L"Қате";
        cannot_open_file_word = L"Файлды ашу мүмкін емес";
        cannot_save_file_word = L"Файлды сақтау мүмкін емес";
        file_is_big_or_error_size_word = L"Файл тым үлкен немесе өлшем қатесі";
        error_convert_text_word = L"Мәтінді түрлендіру қатесі";
        error_regist_class_error_word = L"Класты тіркеу қатесі";
        count_symbols_txt_pole_word = L"Мәтін өрісіндегі таңбалар саны: ";
        select_all_word = L"Барлығын таңдау";
        copy_word = L"Көшіру";
        cut_word = L"Қиып алу";
        redo_word = L"Жоғары";
        undo_word = L"Төмен";
        paste_word = L"Қою";
        save_as_word = L"Басқа сақтау";
        save_word = L"Сақтау";
        open_word = L"Ашу";
        all_files_word = L"Барлық файлдар";
        txt_files_word = L"Мәтіндік файлдар (*.txt)";
        more_word = L"ҚОСЫМША";
        clear_word = L"Тазалау";
        FILE_word = L"XYЙ";
		parametres_word = L"Параметрлер"; // "Parametrlər" (налаштування)
		to_cipher_word = L"Шифрле"; // "Şifrələmək" (шифрувати)
		forward_word = L"ҚОСУ"; // "Qosıw" (вставка)
        to_encrypt_word = L"Шифрлеу"; // "Şifrəlеп" (шифрувати)
        to_decipher_word = L"Шифрді ашу"; // "Şifrdі aşw" (розшифрувати)
        method_word = L"Әдіс"; // "Ädis" (метод)
        break;
    default:
        break;
    }
}