#pragma once

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::IO;

namespace FileEncryptApp {

    public ref class FileEncryptForm : public Form
    {
    public:
        FileEncryptForm(void)
        {
            InitializeComponents();
        }

    private:
        // Элементы управления
        TextBox^ textBoxKey;
        TextBox^ textBoxInputFile;
        TextBox^ textBoxOutputFile;
        Button^ buttonSelectInput;
        Button^ buttonSelectOutput;
        Button^ buttonEncrypt;
        Button^ buttonDecrypt;
        Label^ labelKey;
        Label^ labelInput;
        Label^ labelOutput;
        Label^ labelStatus;

        void InitializeComponents()
        {
            // Настройка формы
            this->Text = L"Шифрование файлов";
            this->Size = System::Drawing::Size(550, 400);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->MaximizeBox = false;
            this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
            this->BackColor = System::Drawing::SystemColors::Control;

            // Метка для ключа
            labelKey = gcnew Label();
            labelKey->Text = L"Ключ шифрования (число):";
            labelKey->Location = System::Drawing::Point(20, 20);
            labelKey->Size = System::Drawing::Size(150, 25);

            // Поле для ввода ключа
            textBoxKey = gcnew TextBox();
            textBoxKey->Location = System::Drawing::Point(180, 20);
            textBoxKey->Size = System::Drawing::Size(100, 25);
            textBoxKey->Text = L"5";

            // Метка для входного файла
            labelInput = gcnew Label();
            labelInput->Text = L"Входной файл:";
            labelInput->Location = System::Drawing::Point(20, 60);
            labelInput->Size = System::Drawing::Size(100, 25);

            // Поле для входного файла
            textBoxInputFile = gcnew TextBox();
            textBoxInputFile->Location = System::Drawing::Point(120, 60);
            textBoxInputFile->Size = System::Drawing::Size(300, 25);
            textBoxInputFile->ReadOnly = true;
            textBoxInputFile->BackColor = System::Drawing::SystemColors::Window;

            // Кнопка выбора входного файла
            buttonSelectInput = gcnew Button();
            buttonSelectInput->Text = L"Выбрать...";
            buttonSelectInput->Location = System::Drawing::Point(430, 60);
            buttonSelectInput->Size = System::Drawing::Size(80, 25);
            buttonSelectInput->Click += gcnew EventHandler(this, &FileEncryptForm::OnSelectInputClick);

            // Метка для выходного файла
            labelOutput = gcnew Label();
            labelOutput->Text = L"Выходной файл:";
            labelOutput->Location = System::Drawing::Point(20, 100);
            labelOutput->Size = System::Drawing::Size(100, 25);

            // Поле для выходного файла
            textBoxOutputFile = gcnew TextBox();
            textBoxOutputFile->Location = System::Drawing::Point(120, 100);
            textBoxOutputFile->Size = System::Drawing::Size(300, 25);

            // Кнопка выбора выходного файла
            buttonSelectOutput = gcnew Button();
            buttonSelectOutput->Text = L"Выбрать...";
            buttonSelectOutput->Location = System::Drawing::Point(430, 100);
            buttonSelectOutput->Size = System::Drawing::Size(80, 25);
            buttonSelectOutput->Click += gcnew EventHandler(this, &FileEncryptForm::OnSelectOutputClick);

            // Кнопка шифрования
            buttonEncrypt = gcnew Button();
            buttonEncrypt->Text = L"Зашифровать";
            buttonEncrypt->Location = System::Drawing::Point(120, 160);
            buttonEncrypt->Size = System::Drawing::Size(130, 40);
            buttonEncrypt->BackColor = System::Drawing::Color::LightGreen;
            buttonEncrypt->Click += gcnew EventHandler(this, &FileEncryptForm::OnEncryptClick);

            // Кнопка дешифрования
            buttonDecrypt = gcnew Button();
            buttonDecrypt->Text = L"Расшифровать";
            buttonDecrypt->Location = System::Drawing::Point(280, 160);
            buttonDecrypt->Size = System::Drawing::Size(130, 40);
            buttonDecrypt->BackColor = System::Drawing::Color::LightCoral;
            buttonDecrypt->Click += gcnew EventHandler(this, &FileEncryptForm::OnDecryptClick);

            // Метка статуса
            labelStatus = gcnew Label();
            labelStatus->Text = L"Готов к работе";
            labelStatus->Location = System::Drawing::Point(20, 230);
            labelStatus->Size = System::Drawing::Size(500, 80);
            labelStatus->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            labelStatus->BackColor = System::Drawing::SystemColors::Info;

            // Добавляем все элементы на форму
            this->Controls->Add(labelKey);
            this->Controls->Add(textBoxKey);
            this->Controls->Add(labelInput);
            this->Controls->Add(textBoxInputFile);
            this->Controls->Add(buttonSelectInput);
            this->Controls->Add(labelOutput);
            this->Controls->Add(textBoxOutputFile);
            this->Controls->Add(buttonSelectOutput);
            this->Controls->Add(buttonEncrypt);
            this->Controls->Add(buttonDecrypt);
            this->Controls->Add(labelStatus);
        }

        // Выбор входного файла
        void OnSelectInputClick(Object^ sender, EventArgs^ e)
        {
            OpenFileDialog^ openDialog = gcnew OpenFileDialog();
            openDialog->Title = L"Выберите файл для шифрования";
            openDialog->Filter = L"Все файлы (*.*)|*.*";

            if (openDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                textBoxInputFile->Text = openDialog->FileName;

                // Автоматически предлагаем имя для выходного файла
                if (textBoxOutputFile->Text == "") {
                    String^ outputFile = Path::GetDirectoryName(openDialog->FileName) +
                        "\\encrypted_" + Path::GetFileName(openDialog->FileName);
                    textBoxOutputFile->Text = outputFile;
                }
            }
        }

        // Выбор выходного файла
        void OnSelectOutputClick(Object^ sender, EventArgs^ e)
        {
            SaveFileDialog^ saveDialog = gcnew SaveFileDialog();
            saveDialog->Title = L"Сохранить как...";
            saveDialog->Filter = L"Все файлы (*.*)|*.*";

            if (saveDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                textBoxOutputFile->Text = saveDialog->FileName;
            }
        }

        // Функция шифрования
        void EncryptFile(String^ inputFile, String^ outputFile, int key)
        {
            // Проверяем существование входного файла
            if (!File::Exists(inputFile)) {
                throw gcnew Exception("Входной файл не найден!");
            }

            // Читаем весь файл
            array<Byte>^ bytes = File::ReadAllBytes(inputFile);

            // Шифруем каждый байт
            for (int i = 0; i < bytes->Length; i++) {
                bytes[i] = bytes[i] + key;
            }

            // Записываем зашифрованные данные
            File::WriteAllBytes(outputFile, bytes);

            labelStatus->Text = L"Шифрование завершено!\n" +
                L"Входной файл: " + inputFile + L"\n" +
                L"Выходной файл: " + outputFile;
        }

        // Функция дешифрования
        void DecryptFile(String^ inputFile, String^ outputFile, int key)
        {
            // Проверяем существование входного файла
            if (!File::Exists(inputFile)) {
                throw gcnew Exception("Входной файл не найден!");
            }

            // Читаем зашифрованный файл
            array<Byte>^ bytes = File::ReadAllBytes(inputFile);

            // Дешифруем каждый байт
            for (int i = 0; i < bytes->Length; i++) {
                bytes[i] = bytes[i] - key;
            }

            // Записываем расшифрованные данные
            File::WriteAllBytes(outputFile, bytes);

            labelStatus->Text = L"Дешифрование завершено!\n" +
                L"Входной файл: " + inputFile + L"\n" +
                L"Выходной файл: " + outputFile;
        }

        // Обработчик кнопки шифрования
        void OnEncryptClick(Object^ sender, EventArgs^ e)
        {
            try {
                // Проверяем ввод ключа
                int key;
                if (!Int32::TryParse(textBoxKey->Text, key)) {
                    MessageBox::Show(L"Введите корректное число в поле ключа!",
                        L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
                    return;
                }

                // Проверяем выбор файлов
                if (String::IsNullOrWhiteSpace(textBoxInputFile->Text)) {
                    MessageBox::Show(L"Выберите входной файл!",
                        L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
                    return;
                }

                if (String::IsNullOrWhiteSpace(textBoxOutputFile->Text)) {
                    MessageBox::Show(L"Укажите выходной файл!",
                        L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
                    return;
                }

                // Шифруем
                labelStatus->Text = L"Шифрование... Пожалуйста, подождите...";
                EncryptFile(textBoxInputFile->Text, textBoxOutputFile->Text, key);

                MessageBox::Show(L"Шифрование успешно завершено!",
                    L"Успех", MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
            catch (Exception^ ex) {
                MessageBox::Show(L"Ошибка: " + ex->Message,
                    L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        // Обработчик кнопки дешифрования
        void OnDecryptClick(Object^ sender, EventArgs^ e)
        {
            try {
                // Проверяем ввод ключа
                int key;
                if (!Int32::TryParse(textBoxKey->Text, key)) {
                    MessageBox::Show(L"Введите корректное число в поле ключа!",
                        L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
                    return;
                }

                // Проверяем выбор файлов
                if (String::IsNullOrWhiteSpace(textBoxInputFile->Text)) {
                    MessageBox::Show(L"Выберите входной файл!",
                        L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
                    return;
                }

                if (String::IsNullOrWhiteSpace(textBoxOutputFile->Text)) {
                    MessageBox::Show(L"Укажите выходной файл!",
                        L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
                    return;
                }

                // Дешифруем
                labelStatus->Text = L"Дешифрование... Пожалуйста, подождите...";
                DecryptFile(textBoxInputFile->Text, textBoxOutputFile->Text, key);

                MessageBox::Show(L"Дешифрование успешно завершено!",
                    L"Успех", MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
            catch (Exception^ ex) {
                MessageBox::Show(L"Ошибка: " + ex->Message,
                    L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }
    private: System::Void InitializeComponent() {
        this->SuspendLayout();
        // 
        // FileEncryptForm
        // 
        this->ClientSize = System::Drawing::Size(284, 261);
        this->Name = L"FileEncryptForm";
        this->Load += gcnew System::EventHandler(this, &FileEncryptForm::FileEncryptForm_Load);
        this->ResumeLayout(false);
    }
private: System::Void FileEncryptForm_Load(System::Object^ sender, System::EventArgs^ e) {
    }
};
}