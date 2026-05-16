cmake_minimum_required(VERSION 3.20)
project(HelloWorld LANGUAGES CXX)

if (NOT MSVC)
    message(FATAL_ERROR "CLR PROJECT REQUIRES MSVC COMPILER!!!!!")
endif()

set(CMAKE_CXX_STANDART 17)
set(CMAKE_CXX_STANDART_REQUIRED ON)

add_executable(HelloWorld WIN32
    main.cpp
    HelloForm.h
    HelloForm.cpp
)

set_target_properties(HelloWorld PROPERTIES
    COMMON_LANGUAGE_RUNTIME ""
    VS_DOTNET_TARGET_FRAMEWORK_VERSION "v4.7.2"
    VS_DOTNET_REFERENCES "System;System.ComponentModel;System.Data;System.Collections;System.Windows.Forms;System.Drawing"
)




$env:Path+="; // путь к BIN cmake "
cmake -G "Version Studio 17" -S . -B build




#pragma once

#using <System.dll>
#using <System.Windows.Forms.dll>
#using <System.Drawing.dll>

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;

ref class Encryptor : public Form {
private:
    TextBox^ textBoxInput;
    TextBox^ textBoxOutput;
    TextBox^ textBoxKey;
    Button^ buttonEncrypt;
    Button^ buttonDecrypt;

public:
    Encryptor() {
        this->Text = L"Crypto App";
        this->Size = Drawing::Size(500, 400);
        this->StartPosition = FormStartPosition::CenterScreen;
        this->FormBorderStyle = FormBorderStyle::FixedSingle;
        this->MaximizeBox = false;

        // Input field
        Label^ labelInput = gcnew Label();
        labelInput->Text = L"Enter text:";
        labelInput->Location = Point(20, 20);
        labelInput->Size = Size(100, 25);

        textBoxInput = gcnew TextBox();
        textBoxInput->Location = Point(20, 50);
        textBoxInput->Size = Size(440, 100);
        textBoxInput->Multiline = true;

        // Key field
        Label^ labelKey = gcnew Label();
        labelKey->Text = L"Key (1-10):";
        labelKey->Location = Point(20, 170);
        labelKey->Size = Size(100, 25);

        textBoxKey = gcnew TextBox();
        textBoxKey->Location = Point(130, 170);
        textBoxKey->Size = Size(50, 25);
        textBoxKey->Text = L"3";

        // Buttons
        buttonEncrypt = gcnew Button();
        buttonEncrypt->Text = L"Crypto";
        buttonEncrypt->Location = Point(100, 210);
        buttonEncrypt->Size = Size(130, 40);
        buttonEncrypt->Click += gcnew EventHandler(this, &Encryptor::OnEncrypt);

        buttonDecrypt = gcnew Button();
        buttonDecrypt->Text = L"Decrypto";
        buttonDecrypt->Location = Point(260, 210);
        buttonDecrypt->Size = Size(130, 40);
        buttonDecrypt->Click += gcnew EventHandler(this, &Encryptor::OnDecrypt);

        // Output field
        Label^ labelOutput = gcnew Label();
        labelOutput->Text = L"Result:";
        labelOutput->Location = Point(20, 270);
        labelOutput->Size = Size(100, 25);

        textBoxOutput = gcnew TextBox();
        textBoxOutput->Location = Point(20, 300);
        textBoxOutput->Size = Size(440, 60);
        textBoxOutput->Multiline = true;
        textBoxOutput->ReadOnly = true;

        // Add controls
        this->Controls->Add(labelInput);
        this->Controls->Add(textBoxInput);
        this->Controls->Add(labelKey);
        this->Controls->Add(textBoxKey);
        this->Controls->Add(buttonEncrypt);
        this->Controls->Add(buttonDecrypt);
        this->Controls->Add(labelOutput);
        this->Controls->Add(textBoxOutput);
    }

    void OnEncrypt(Object^ sender, EventArgs^ e) {
        if (textBoxInput->Text == L"") {
            MessageBox::Show(L"Please enter text!", L"Error");
            return;
        }

        int key = Convert::ToInt32(textBoxKey->Text);
        if (key < 1 || key > 10) {
            MessageBox::Show(L"Key must be between 1 and 10!", L"Error");
            return;
        }

        String^ result = L"";
        for (int i = 0; i < textBoxInput->Text->Length; i++) {
            result += (wchar_t)(textBoxInput->Text[i] + key);
        }

        textBoxOutput->Text = result;
        MessageBox::Show(L"Encryption completed!", L"Success");
    }

    void OnDecrypt(Object^ sender, EventArgs^ e) {
        if (textBoxOutput->Text == L"") {
            MessageBox::Show(L"No encrypted text!", L"Error");
            return;
        }

        int key = Convert::ToInt32(textBoxKey->Text);
        if (key < 1 || key > 10) {
            MessageBox::Show(L"Key must be between 1 and 10!", L"Error");
            return;
        }

        String^ result = L"";
        for (int i = 0; i < textBoxOutput->Text->Length; i++) {
            result += (wchar_t)(textBoxOutput->Text[i] - key);
        }

        textBoxOutput->Text = result;
        MessageBox::Show(L"Decryption completed!", L"Success");
    }
private: System::Void InitializeComponent() {
    this->SuspendLayout();
    // 
    // Encryptor
    // 
    this->ClientSize = System::Drawing::Size(284, 261);
    this->Name = L"Encryptor";
    this->Load += gcnew System::EventHandler(this, &Encryptor::Encryptor_Load);
    this->ResumeLayout(false);
}
private: System::Void Encryptor_Load(System::Object^ sender, System::EventArgs^ e) {
}
};
