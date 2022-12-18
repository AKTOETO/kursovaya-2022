﻿#include "menu.h"

Menu::Menu()
{
	// выделение памяти под массив с описанием функций
	m_command = new Command[NUMBER_OF_COMMANDS];

	// чтение команд из файлов
	for (int i = 0; i < NUMBER_OF_COMMANDS; i++)
	{
		m_command[i].FillCommandData(COMMAND_DESCRIPTION_FILES[i]);
	}

	// заполнение массива функций обработки команд
	m_command[0].SetCheckFunction(&Menu::CheckCMDExit);
	m_command[1].SetCheckFunction(&Menu::CheckCMDHelp);
	m_command[2].SetCheckFunction(&Menu::CheckCMDReadDB);
	m_command[3].SetCheckFunction(&Menu::CheckCMDPrintDBToConsole);
	m_command[4].SetCheckFunction(&Menu::CheckCMDSaveDBToFile);
	m_command[5].SetCheckFunction(&Menu::CheckCMDDeleteDBNode);
	m_command[6].SetCheckFunction(&Menu::CheckCMDClearDB);
	m_command[7].SetCheckFunction(&Menu::CheckCMDSelectFromDB);
	m_command[8].SetCheckFunction(&Menu::CheckCMDReplaceDefaultDB);
	m_command[9].SetCheckFunction(&Menu::CheckCMDSortDB);
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
				INFO("\"" + input_first_command + "\"" + NOT_CORRECT_COMMAND);
			}
		}

	} while (input_first_command != CMD_NAME(0));
}

bool Menu::IsCommandCorrect(const string& _command)
{
	// проходимся по массиву команд
	for (int i = 0; i < NUMBER_OF_COMMANDS; i++)
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
	for (int i = 0; i < NUMBER_OF_COMMANDS; i++)
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
	// параметров у нее нет, так что их 
	// не надо проверять 
}

// проверка команды ПОМОЩЬ
void Menu::CheckCMDHelp(string _str)
{
	// если аргументы не были переданы
	// тогда выводим все команды и информацию по ним
	if (!_str.length())
	{
		for (int i = 0; i < NUMBER_OF_COMMANDS; i++)
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
			cout << "\t\"" << _str << "\"" << NOT_CORRECT_COMMAND;
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
		else
		{
			temp = DB_FOLDER_PATH + temp;
			INFO("Использование файла " + temp);
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
			m_db_manager.ReadDBFromFile(fin);
		}

		// закрытие файла
		fin.close();
	}
	// из консоли
	else if (temp == "-к")
	{
		INFO("Ввод с консоли:");
		MusicStuff ms;

		// считывание места хранения
		ms.SetStorage(
			CheckableRead(
				"\t[Введите НОСИТЕЛЬ]> "
			)
		);

		// запись порядкового номера
		ms.SetSerialNumber(
			m_db_manager.GetLastIndexOfNode() + 1
		);

		// считывание названия трека
		ms.SetName(
			CheckableRead(
				"\t[Введите НАЗВАНИЕ ТРЕКА]> "
			)
		);

		// считывание имени исполнителя
		ms.SetArtistsName(
			CheckableRead(
				"\t[Введите ИМЯ исполнителя]> "
			)
		);

		// считывание фамилии исполнителя
		ms.SetArtistsSurname(
			CheckableRead(
				"\t[Введите ФАМИЛИЮ исполнителя]> "
			)
		);

		// считывание времени проигрывания трека
		ms.SetSoundTime(
			atoi(
				CheckableRead(
					"\t[Введите ВРЕМЯ ЗВУЧАНИЯ (минуты)]> ",
					IsThereANotNegativeNumber
				).c_str()
			)
		);

		// считывание количества воспроизведений
		ms.SetNumberOfPlays(
			atoi(
				CheckableRead(
					"\t[Введите КОЛИЧЕСТВО ВОСПРОИЗВЕДЕНИЙ]> ",
					IsThereANotNegativeNumber
				).c_str()
			)
		);

		// считывание цены
		ms.SetPrice(
			atoi(
				CheckableRead(
					"\t[Введите ЦЕНУ (рубли)]> ",
					IsThereANotNegativeNumber
				).c_str()
			)
		);

		// запись считанного элемента в список
		m_db_manager.ReadDBNodeFromNode(ms);
	}
	else
	{
		INFO("CHECKCMDREADDB: неизвестный ключ: \"" + temp + "\"");
	}
}

// сохранение базы данных в файл
void Menu::CheckCMDSaveDBToFile(string _str)
{
	if (!m_db_manager.GetSizeOfDataBase())
	{
		FUNC_INFO("база данных пуста");
		return;
	}
	// место считывания информации
	string temp = GetToken(_str);

	// создаем файловый поток и
	// выводим информацию туда
	ofstream fout;

	// если ключей нет
	if (!temp.length())
	{
		// использую стандартный файл вывода
		INFO("Использование стандартного файла вывода " + DB_FILE_PATH);
		temp = DB_FILE_PATH;

		// открытие файла
		fout.open(temp);

		// сохранение базы данных в файл
		m_db_manager.SaveDBToFile(fout);
	}
	// иначе пытаюсь вывести в temp файл
	else if (temp == "-ф")
	{
		INFO("Печать в файл");

		// получение файлового пути
		temp = DB_FOLDER_PATH + GetToken(_str);

		INFO("Использование файла " + temp);

		// открытие файла
		fout.open(temp);

		// сохранение базы данных в файл
		m_db_manager.SaveDBToFile(fout);
	}
	else
	{
		INFO("CHECKCMDSAVEDBTOFILE: неизвестный ключ: \"" + temp + "\"");
	}
	// закрытие файлового потока
	fout.close();
}

// удаление элеемнта из базы данных
void Menu::CheckCMDDeleteDBNode(string _str)
{
	if (!m_db_manager.GetSizeOfDataBase())
	{
		FUNC_INFO("база данных пуста");
		return;
	}

	// как удалять: с помощью индекса
	// или с помощью поля
	string temp = GetToken(_str);

	// если было введено удаление по индексу
	if (temp == "-и")
	{
		// получение индекса
		temp = GetToken(_str);

		// если есть индекс и temp число
		if (temp.length() && IsThereANotNegativeNumber(temp))
		{
			// запись индекса в виде числа
			int index = atoi(temp.c_str()) - 1;

			// поиск элемента для удаления
			node<MusicStuff>* nd = m_db_manager.FindNodeToDelete(index);

			if (!nd)
			{
				FUNC_INFO("элемент не найден");
				return;
			}

			// получение токена
			temp = GetToken(_str);

			// удаляем без диалогов
			if (temp == "-с")
			{
				m_db_manager.DeleteDBNode(nd);
			}
			// удаляем с диалогом с пользователем
			else
			{
				// уточнение у пользователя
				// готов ли он удалить эти элементы			
				INFO("\tЭлемент для удаления:");
				cout << nd->get_data() << endl;

				// получение ответа
				string answ = CheckableRead(
					"\t[Готовы ли вы удалить его? (да/нет)]> ",
					[](string str)
					{
						return ToLowerCase(str) == "да" || ToLowerCase(str) == "нет";
					}
				);

				// если ответ да
				if (answ == "да")
				{
					m_db_manager.DeleteDBNode(nd);
				}
				else
				{
					INFO("Элемнет не удалены")
				}
			}
		}
		// если нет индекса
		else
		{
			FUNC_INFO("не введено значение после -и или оно некорректно");
		}
	}
	// если было введено удаление по полю
	else if (temp == "-к")
	{
		// процедура выборки
		CheckCMDSelectFromDB("");

		// получение ответа
		string answ = CheckableRead(
			"\t[Готовы ли вы удалить их? (да/нет)]> ",
			[](string str)
			{
				return ToLowerCase(str) == "да" || ToLowerCase(str) == "нет";
			}
		);

		if(answ == "да")
		{
			// удаление выбранных элементов из списка
			m_db_manager.DeleteDBSelectedList();
		}
		else
		{
			INFO("Элемнеты не удалены")
		}
	}
	else
	{
		INFO("CHECKCMDDELETEDBNODE: неизвестный ключ: \"" + temp + "\"");
	}

}

// проверка команды	ПЕЧАТЬДАННЫХ
void Menu::CheckCMDPrintDBToConsole(string _str)
{
	if (!m_db_manager.GetSizeOfDataBase())
	{
		FUNC_INFO("база данных пуста");
		return;
	}
	m_db_manager.PrintDBToConsole();
}

// очистка базы данных
void Menu::CheckCMDClearDB(string _str)
{
	INFO("База данных очищена")
	m_db_manager.ClearDB();
}

// выбрать из базы данных определенны элементы
void Menu::CheckCMDSelectFromDB(string _str)
{
	if (!m_db_manager.GetSizeOfDataBase())
	{
		FUNC_INFO("база данных пуста");
		return;
	}

	// взятие ключа
	string key = GetToken(_str);

	// Вывод названий полей в базе данных
	PrintFieldsOfDataBase();

	// индекс поля
	int number_of_field =
		atoi(
			CheckableRead(
				"\t[По какому полю осуществлять выбор]> ",
				[](string num)
				{
					return
					IsThereANumber(num) &&
					1 <= atoi(num.c_str()) &&
					atoi(num.c_str()) <= NUMBER_OF_FIELDS;
				}
	).c_str()) - 1;

	// получение списка значений поля
	my_list<string> out = *m_db_manager.GetDataInField(number_of_field);

	// печать списка
	cout << endl;
	int ind = 1;
	out.for_each([&ind](node<string>* el)
		{
			cout << "\t" << ind++ << ") " << el->get_data() << "\n";
		}
	);

	// какое значение искать в поле
	int number_of_value =
		atoi(
			CheckableRead(
				"\t[Какое значение искать в поле]> ",
				[&out](string num)
				{
					return
					IsThereANumber(num) &&
					1 <= atoi(num.c_str()) &&
					atoi(num.c_str()) <= out.get_size();
				}
	).c_str()) - 1;

	// выборка элементов списка с определенным
	// значением определенного поля
	m_db_manager.SelectDB
	(
		number_of_field,
		out.get_element_by_index(number_of_value)->get_data()
	);

	cout << "\n[Выбраны следующие элементы]> \n";

	// печать списка
	m_db_manager.GetSelectedList()->for_each([](auto _el)
		{
			cout << _el->get_data()->get_data() << endl;
		}
	);
}

// заменить исходную бд полученной из выборки
void Menu::CheckCMDReplaceDefaultDB(string _str)
{
	if (!m_db_manager.GetSizeOfDataBase())
	{
		FUNC_INFO("база данных пуста");
		return;
	}

	else

	{
		// вызов функции определяющей выбранные элемента
		CheckCMDSelectFromDB("");

		// получение ответа
		string answ = CheckableRead(
			"\t[Готовы ли вы оставить только эти элементы? (да/нет)]> ",
			[](string str)
			{
				return ToLowerCase(str) == "да" || ToLowerCase(str) == "нет";
			}
		);

		if (answ == "да")
		{
			INFO("Замена текущей базы");
			m_db_manager.ReplaceDefaultDataBase();
		}
		else
		{
			INFO("Элемнеты не заменены")
		}
	}

}

void Menu::CheckCMDSortDB(string _str)
{
	//TODO добавить сортировку
}
