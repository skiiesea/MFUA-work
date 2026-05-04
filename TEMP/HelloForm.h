#pragma once

namespace HelloWorld {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// —водка дл€ HelloForm
	/// </summary>
	public ref class HelloForm : public System::Windows::Forms::Form
	{
	public:
		HelloForm(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
		}

	protected:
		/// <summary>
		/// ќсвободить все используемые ресурсы.
		/// </summary>
		~HelloForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ LabelCounter;
	private: System::Windows::Forms::Button^ ButtonCounter;
	private: int counter = 0;
	protected:

	protected:


	private:
		/// <summary>
		/// ќб€зательна€ переменна€ конструктора.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// “ребуемый метод дл€ поддержки конструктора Ч не измен€йте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->LabelCounter = (gcnew System::Windows::Forms::Label());
			this->ButtonCounter = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = false;
			this->label1->Location = System::Drawing::Point(164, 132);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(62, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Hello World";
			// 
			// LabelCounter
			// 
			this->LabelCounter->AutoSize = true;
			this->LabelCounter->Location = System::Drawing::Point(194, 154);
			this->LabelCounter->Name = L"LabelCounter";
			this->LabelCounter->Size = System::Drawing::Size(13, 13);
			this->LabelCounter->TabIndex = 1;
			this->LabelCounter->Text = L"0";
			// 
			// ButtonCounter
			// 
			this->ButtonCounter->Font = (gcnew System::Drawing::Font(L"MS Reference Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->ButtonCounter->Location = System::Drawing::Point(140, 179);
			this->ButtonCounter->Name = L"ButtonCounter";
			this->ButtonCounter->Size = System::Drawing::Size(124, 30);
			this->ButtonCounter->TabIndex = 2;
			this->ButtonCounter->Text = L"PRESS";
			this->ButtonCounter->UseVisualStyleBackColor = true;
			this->ButtonCounter->Click += gcnew System::EventHandler(this, &HelloForm::ButtonCounter_Click);
			// 
			// HelloForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::Info;
			this->ClientSize = System::Drawing::Size(1920, 1061);
			this->Controls->Add(this->ButtonCounter);
			this->Controls->Add(this->LabelCounter);
			this->Controls->Add(this->label1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"HelloForm";
			this->Text = L"HelloForm";
			this->Load += gcnew System::EventHandler(this, &HelloForm::HelloForm_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void HelloForm_Load(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void label2_Click(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void ButtonCounter_Click(System::Object^ sender, System::EventArgs^ e) {
		++counter;
		LabelCounter->Text = System::Convert::ToString(counter);
	}
	};
}
