#pragma once
#include "Processor.h"
#include <msclr\marshal_cppstd.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <fstream>

namespace Angionalysis {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::Label^  label1;

	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Label^  label2;



	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Bookman Old Style", 30, System::Drawing::FontStyle::Bold));
			this->label1->ForeColor = System::Drawing::SystemColors::ButtonHighlight;
			this->label1->Location = System::Drawing::Point(466, 80);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(668, 128);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Angionalysis";
			this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->label1->UseCompatibleTextRendering = true;
			this->label1->Click += gcnew System::EventHandler(this, &MyForm::label1_Click);
			// 
			// button1
			// 
			this->button1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.1F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button1->Location = System::Drawing::Point(1100, 240);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(240, 60);
			this->button1->TabIndex = 1;
			this->button1->Text = L"Select Image";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(272, 246);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(796, 38);
			this->textBox1->TabIndex = 2;
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"image.tif";
			// 
			// button2
			// 
			this->button2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.900001F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button2->Location = System::Drawing::Point(680, 400);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(240, 100);
			this->button2->TabIndex = 3;
			this->button2->Text = L"Analyze";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click_1);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.900001F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label2->ForeColor = System::Drawing::SystemColors::Control;
			this->label2->Location = System::Drawing::Point(705, 626);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(0, 39);
			this->label2->TabIndex = 4;
			this->label2->Click += gcnew System::EventHandler(this, &MyForm::label2_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(16, 31);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->ClientSize = System::Drawing::Size(1568, 912);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->label1);
			this->Name = L"MyForm";
			this->Text = L"Angionalysis";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void label1_Click(System::Object^  sender, System::EventArgs^  e) 
	{

	}
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		openFileDialog1->ShowDialog();
		textBox1->Text = openFileDialog1->FileName;
	}

	private: System::Void button2_Click_1(System::Object^  sender, System::EventArgs^  e) 
	{
		//Call analyze picture whose path is given from textbox
		Processor proc;
		label2->Text = "Analyzing...";
		bool worked = proc.updatePicture(msclr::interop::marshal_as<std::string>(textBox1->Text));
		if (worked)
			label2->Text = "Analysis complete...";
		else
			label2->Text = "Failed to read file!";
		if (worked)
		{
			proc.enhance();
			proc.zoom();
			proc.fillContours();
			int meshes = proc.countMeshes();
			label2->Text = gcnew String((std::string("Meshes found: ") + std::to_string(meshes)).c_str());
			proc.show("post");
		}
	}
private: System::Void label2_Click(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e) {
}
};
}
