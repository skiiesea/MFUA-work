#pragma once

#include <cmath>
#include <string>
#include <vector>
#include <vcclr.h>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Threading;

namespace CryptoSquareApp {

    public ref class CryptoForm : public System::Windows::Forms::Form
    {
    public:
        CryptoForm(void)
        {
            InitializeComponent();
        }

    protected:
        ~CryptoForm()
        {
            if (components)
            {
                delete components;
            }
        }

    private:
        System::Windows::Forms::TextBox^ textBoxInput;
        System::Windows::Forms::TextBox^ textBoxOutput;
        System::Windows::Forms::Button^ buttonEncrypt;
        System::Windows::Forms::Button^ buttonDecrypt;
        System::Windows::Forms::Label^ labelInput;
        System::Windows::Forms::Label^ labelOutput;

        static const wchar_t PADDING_CHAR = (wchar_t)11;

        System::ComponentModel::Container^ components;

        std::wstring StringToWString(String^ str)
        {
            if (String::IsNullOrEmpty(str))
                return std::wstring();

            pin_ptr<const wchar_t> ptr = PtrToStringChars(str);
            return std::wstring(ptr, str->Length);
        }

        String^ WStringToString(const std::wstring& wstr)
        {
            return gcnew String(wstr.c_str());
        }

        String^ EncryptText(String^ input)
        {
            if (String::IsNullOrEmpty(input))
                throw gcnew ArgumentException("Текст не может быть пустым!");

            if (input->Length > 1000000)
                throw gcnew ArgumentException("Текст слишком большой! Максимум 1,000,000 символов.");

            std::wstring text = StringToWString(input);

            int L = text.length();

            int n = (int)ceil(sqrt((double)L));
            int matrixSize = n * n;

            std::wstring padded = text;
            padded.resize(matrixSize, PADDING_CHAR);

            std::vector<std::wstring> rows;
            for (int i = 0; i < n; i++) {
                rows.push_back(padded.substr(i * n, n));
            }

            std::vector<std::wstring> rotated;
            for (int i = 0; i < n; i++) {
                std::wstring newRow;
                for (int j = 0; j < n; j++) {
                    newRow += rows[n - 1 - j][i];
                }
                rotated.push_back(newRow);
            }

            std::wstring result;
            for (int i = 0; i < n; i++) {
                result += rotated[i];
                if (i != n - 1)
                    result += L'\n';
            }

            return WStringToString(result);
        }

        String^ DecryptText(String^ encrypted)
        {
            if (String::IsNullOrEmpty(encrypted))
                throw gcnew ArgumentException("Нет текста для расшифровки!");

            array<String^>^ lines = encrypted->Split(L'\n');
            int n = lines->Length;

            if (n == 0)
                throw gcnew ArgumentException("Неверный формат!");

            int rowLen = lines[0]->Length;
            for (int i = 1; i < lines->Length; i++) {
                if (lines[i]->Length != rowLen)
                    throw gcnew ArgumentException("Текст поврежден!");
            }

            if (rowLen != n)
                throw gcnew ArgumentException("Текст не квадратный!");

            std::vector<std::wstring> matrix;
            for (int i = 0; i < n; i++) {
                matrix.push_back(StringToWString(lines[i]));
            }

            std::vector<std::wstring> rotated;
            for (int i = 0; i < n; i++) {
                std::wstring newRow;
                for (int j = 0; j < n; j++) {
                    newRow += matrix[j][n - 1 - i];
                }
                rotated.push_back(newRow);
            }

            std::wstring result;
            for (int i = 0; i < n; i++) {
                result += rotated[i];
            }

            while (!result.empty() && result.back() == PADDING_CHAR) {
                result.pop_back();
            }

            return WStringToString(result);
        }

        void SetOutputText(String^ text)
        {
            if (this->InvokeRequired) {
                this->BeginInvoke(gcnew Action<String^>(this, &CryptoForm::SetOutputText), text);
                return;
            }
            textBoxOutput->Text = text;
        }

        void SetOutputReadOnly(bool readOnly)
        {
            if (this->InvokeRequired) {
                this->BeginInvoke(gcnew Action<bool>(this, &CryptoForm::SetOutputReadOnly), readOnly);
                return;
            }
            textBoxOutput->ReadOnly = readOnly;
            if (readOnly)
                textBoxOutput->BackColor = SystemColors::ControlLight;
            else
                textBoxOutput->BackColor = SystemColors::Window;
        }

        void EncryptWorker()
        {
            try {
                SetOutputReadOnly(true);
                String^ input = textBoxInput->Text;
                String^ encrypted = EncryptText(input);
                SetOutputText(encrypted);
            }
            catch (Exception^ ex) {
                MessageBox::Show(ex->Message, "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
            finally {
                SetOutputReadOnly(false);
            }
        }

        void DecryptWorker()
        {
            try {
                SetOutputReadOnly(true);
                String^ encrypted = textBoxOutput->Text;
                String^ decrypted = DecryptText(encrypted);
                SetOutputText(decrypted);
            }
            catch (Exception^ ex) {
                MessageBox::Show(ex->Message, "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
            finally {
                SetOutputReadOnly(false);
            }
        }

        void InitializeComponent(void)
        {
            this->textBoxInput = (gcnew System::Windows::Forms::TextBox());
            this->textBoxOutput = (gcnew System::Windows::Forms::TextBox());
            this->buttonEncrypt = (gcnew System::Windows::Forms::Button());
            this->buttonDecrypt = (gcnew System::Windows::Forms::Button());
            this->labelInput = (gcnew System::Windows::Forms::Label());
            this->labelOutput = (gcnew System::Windows::Forms::Label());
            this->SuspendLayout();

            this->textBoxInput->Location = System::Drawing::Point(20, 40);
            this->textBoxInput->Multiline = true;
            this->textBoxInput->Size = System::Drawing::Size(460, 120);
            this->textBoxInput->ScrollBars = System::Windows::Forms::ScrollBars::None;
            this->textBoxInput->Font = (gcnew System::Drawing::Font(L"Consolas", 10));
            this->textBoxInput->WordWrap = true;
            this->textBoxInput->TabIndex = 0;

            this->textBoxOutput->Location = System::Drawing::Point(20, 200);
            this->textBoxOutput->Multiline = true;
            this->textBoxOutput->Size = System::Drawing::Size(460, 120);
            this->textBoxOutput->ReadOnly = true;
            this->textBoxOutput->ScrollBars = System::Windows::Forms::ScrollBars::None;
            this->textBoxOutput->Font = (gcnew System::Drawing::Font(L"Consolas", 10));
            this->textBoxOutput->WordWrap = true;
            this->textBoxOutput->BackColor = System::Drawing::SystemColors::ControlLight;
            this->textBoxOutput->TabIndex = 1;

            this->buttonEncrypt->Location = System::Drawing::Point(120, 340);
            this->buttonEncrypt->Size = System::Drawing::Size(110, 40);
            this->buttonEncrypt->Text = L"Шифровать";
            this->buttonEncrypt->UseVisualStyleBackColor = true;
            this->buttonEncrypt->TabIndex = 2;
            this->buttonEncrypt->Click += gcnew System::EventHandler(this, &CryptoForm::buttonEncrypt_Click);

            this->buttonDecrypt->Location = System::Drawing::Point(270, 340);
            this->buttonDecrypt->Size = System::Drawing::Size(110, 40);
            this->buttonDecrypt->Text = L"Дешифровать";
            this->buttonDecrypt->UseVisualStyleBackColor = true;
            this->buttonDecrypt->TabIndex = 3;
            this->buttonDecrypt->Click += gcnew System::EventHandler(this, &CryptoForm::buttonDecrypt_Click);

            this->labelInput->AutoSize = true;
            this->labelInput->Location = System::Drawing::Point(20, 20);
            this->labelInput->Text = L"Исходный текст:";
            this->labelInput->TabIndex = 4;

            this->labelOutput->AutoSize = true;
            this->labelOutput->Location = System::Drawing::Point(20, 180);
            this->labelOutput->Text = L"Зашифрованный текст:";
            this->labelOutput->TabIndex = 5;

            this->ClientSize = System::Drawing::Size(500, 410);
            this->Controls->Add(this->labelOutput);
            this->Controls->Add(this->labelInput);
            this->Controls->Add(this->buttonDecrypt);
            this->Controls->Add(this->buttonEncrypt);
            this->Controls->Add(this->textBoxOutput);
            this->Controls->Add(this->textBoxInput);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->MaximizeBox = false;
            this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
            this->Text = L"Шифрование квадратом 90°";

            this->ResumeLayout(false);
            this->PerformLayout();
        }

        void buttonEncrypt_Click(System::Object^ sender, System::EventArgs^ e)
        {
            if (String::IsNullOrWhiteSpace(textBoxInput->Text)) {
                MessageBox::Show("Введите текст для шифрования!", "Предупреждение",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            Thread^ encryptThread = gcnew Thread(gcnew ThreadStart(this, &CryptoForm::EncryptWorker));
            encryptThread->Start();
        }

        void buttonDecrypt_Click(System::Object^ sender, System::EventArgs^ e)
        {
            if (String::IsNullOrWhiteSpace(textBoxOutput->Text)) {
                MessageBox::Show("Нет зашифрованного текста для расшифровки!", "Предупреждение",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            Thread^ decryptThread = gcnew Thread(gcnew ThreadStart(this, &CryptoForm::DecryptWorker));
            decryptThread->Start();
        }
    };
}