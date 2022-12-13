﻿#include "menu.h"

Menu::Menu()
{
	// выделение памяти под массив с описанием функций
	m_command = new Command[NUMBERS_OF_COMMANDS];

	// чтение команд из файлов
	for (int i = 0; i < NUMBERS_OF_COMMANDS; i++)
	{
		m_command[i].FillCommandData(COMMAND_DESCRIPTION_FILES[i]);
	}

	// заполнение массива функций обработки команд
	m_command[0].SetCheckFunction(&Menu::CheckCMDExit);
	m_command[1].SetCheckFunction(&Menu::CheckCMDHelp);
	m_command[2].SetCheckFunction(&Menu::CheckCMDReadDB);
	m_command[3].SetCheckFunction(&Menu::CheckCMDPrintDBToConsole);
}

Menu::~Menu()
{
	delete[] m_command;
}

void Menu::ProgramMenu()
{

	// вывод справки о программе
	cout << DBMS_DESCRIPTION;

	// строка с введенными командами
	string input_all_command;
	// строка с введенными командами
	string input_first_command;
	// строка с введенными аттрибутами команды
	string input_attributes;

	// цикл выполнения программы
	do
	{
		// вывод сообщения запроса команды
		cout << DBMS_CONSOLE_REQUEST_COMMAND;

		// считывание команды
		getline(cin, input_all_command);

		// если строка не пустая,
		// тогда можно проверять ее на корректность
		if (input_all_command.length() != 0)
		{
			// подготовка строки для получения слов из нее
			RemoveUnnecessarySpaces(input_all_command);

			// перевод строки в нижний регистр
			input_all_command = ToLowerCase(input_all_command);

			// получение только команды (без атрибутов)
			input_first_command = GetToken(input_all_command, ' ');

			// если введенное слово является командой
			// и не был введен выход 
			
			if (
				IsCommandCorrect(input_first_command) &&
				input_first_command != CMD_NAME(0)
				)
			{
				//вызов необходимой функции для команды
				CMD_CHK_FUNC(input_first_command, input_all_command);
			}
			// если была введена не команда
			else if (input_first_command != CMD_NAME(0))
			{
				INFO("\""+input_first_command+"\"" + NOT_CORRECT_COMMAND);
			}
		}
	} while (input_first_command != CMD_NAME(0));
}

bool Menu::IsCommandCorrect(const string& _command)
{
	// проходимся по массиву команд
	for (int i = 0; i < NUMBERS_OF_COMMANDS; i++)
	{
		// если нашли совпадающую команду
		// выводим 1
		if (CMD_NAME(i) == _command)
		{
			return true;
		}
	}

	// иначе выводим 0
	return false;
}

int Menu::GetNumberOfCommand(const string& _command)
{
	// проходимся по массиву команд
	for (int i = 0; i < NUMBERS_OF_COMMANDS; i++)
	{
		// если нашли совпадающую команду
		// выводим i
		if (CMD_NAME(i) == _command)
		{
			return i;
		}
	}

	// иначе -1
	return -1;
}

// проверка команды выход
void Menu::CheckCMDExit(string _str)
{
	// параметров у нее нат, так что их 
	// не надо проверять 
}

// проверка команды ПОМОЩЬ
void Menu::CheckCMDHelp(string _str)
{
	// если аргументы не были переданы
	// тогда выводим все команды и информацию по ним
	if (_str.size() == 0)
	{
		for (int i = 0; i < NUMBERS_OF_COMMANDS; i++)
		{
			cout << ToUpperCase(CMD_NAME(i))
				<< "\t\t" << CMD_SH_DECR(i)
				<< endl;
		}
	}
	// если есть какие-то аргументы
	else
	{
		// если этот аргумент - команда
		// выводим подробную инфу по ней
		if (IsCommandCorrect(_str))
		{
			cout << CMD_FL_DESCR(GetNumberOfCommand(_str)) << endl;
		}
		// если такого аргумента не существует
		else
		{
			cout << "\t" << _str << NOT_CORRECT_COMMAND;
		}
	}
}

// проверка команды	ЧТЕНИЕДАННЫХ
void Menu::CheckCMDReadDB(string _str)
{
	// место считывания информации
	string temp = GetToken(_str);

	// проверка места считывания информации
	// из файла
	if (temp == "-ф" || !temp.length())
	{
		INFO("Чтение из файла");

		// проверка на наличие файлового пути
		temp = GetToken(_str);

		// если путь до файла не указан
		// используем стандартный путь
		if (temp.length() == 0)
		{
			INFO("Использование стандартного файла " + DB_FILE_PATH);
			temp = DB_FILE_PATH;
		}

		// создаем файловый поток и
		// читаем информацию оттуда
		ifstream fin(temp);

		// если не удалось открыть файл
		if (!fin.is_open())
		{
			// вывод сообщение об ошибке
			INFO("Файл \"" + temp + "\" не был открыт");
		}
		// иначе записываем информацию из файлв в консоль
		else
		{
			m_music_list.ReadDBFromFile(fin);
		}

		// закрытие файла
		fin.close();
	}
	// из консоли
	else if (temp == "-к")
	{
		//TODO
	}
	else
	{
		INFO("CHECKREADDATA: неизвестный ключ: \"" + temp+"\"");
	}
}

void Menu::CheckCMDSaveDBToFile(string _str)
{
}

void Menu::CheckCMDDeleteDBNode(string _str)
{
}

// проверка команды	ПЕЧАТЬДАННЫХ
void Menu::CheckCMDPrintDBToConsole(string _str)
{
	// ДОЛЖНО БЫТЬ НЕ ТАК
	// НАДО ПРОСТО ВЫВОДИТЬ В КОНСОЛЬ, НИЧЕГО
	// НЕ ВЫБИРАЯ ИФАМИ
	//// место вывода информации
	//string temp = GetToken(_str);

	//// создаем файловый поток и
	//// выводим информацию туда
	//ofstream fout;

	//// проверка места вывода информации
	//// в файла
	//if (temp == "-ф")
	//{
	//	INFO("Печать в файл");

	//	// проверка на наличие файлового пути
	//	temp = GetToken(_str);

	//	// если путь до файла не указан
	//	// используем стандартный путь
	//	if (temp.length() == 0)
	//	{
	//		INFO("Использование стандартного файла " + DB_OUTPUT_PATH);
	//		temp = DB_OUTPUT_PATH;
	//	}

	//	m_music_list.PrintDBToConsole(fout);		

	//	// закрытие файла
	//	fout.close();
	//}
	//// в консоли
	//else if (temp == "-к" || !temp.length())
	//{
	//	//TODO
	//	m_music_list.PrintDBToConsole(cout);
	//}
	//else
	//{
	//	INFO("CHECKPRINTDATA: неизвестный ключ: \"" + temp + "\"");
	//}

}

void Menu::CheckCMDClearDB(string _str)
{
}

void Menu::CheckCMDSelectFromDB(string _str)
{
}