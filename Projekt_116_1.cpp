// Projekt_116_1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <conio.h>
#include <Windows.h>

#define KEY_UP 72 // przypisanie klawiszy strzałek do sterowania figurą
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

using namespace std;

void gotoxy(int x, int y); // Funkcja gotoxy(int x, int y) ustawia kursor w punkcie (x,y) konsoli
void HideCursor(); // Funckja ukrywająca kursor w konsoli
void rysowanie(int &ASCII, int &a, int &x, int &y); // rysowanie oraz przekształcenia figury
void wczyt_dane(int &ASCII, int &a); // wczytanie rozmiaru oraz znaku ASCII
void rozmiar_konsoli(int &columns, int &rows); // wczytanie aktualnego rozmiaru konsoli do dynamicznego ograniczania operacji na figurze 


int main()
{
	setlocale(LC_ALL, ""); // Polskie znaki

	int x, y; // położenie figury
	int znak; // znak ustawiony jest na wybór numeru ASCII do którego przypisany jest znak, np. 43 to *
	int a; // wielkość figury
	// wywołanie funkcji. Funckja rysowanie jest dosyć duża, ale miałem problemy z jej podziałem na mniejsze funckje, więc zostawiłem ją w takiej formie
	wczyt_dane(znak, a); 
	rysowanie(znak, a, x, y);

	_getch();
	return 0;
}
//----------------------------------------------------------------------------
void gotoxy(int x, int y)
{
	COORD c;
	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
//---------------------------------------------------------------------------- 
void HideCursor()
{
	::HANDLE hConsoleOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
	::CONSOLE_CURSOR_INFO hCCI;
	::GetConsoleCursorInfo(hConsoleOut, &hCCI);
	hCCI.bVisible = FALSE;
	::SetConsoleCursorInfo(hConsoleOut, &hCCI);
}
//-----------------------------------------------------------------------------
void rysowanie(int &ASCII, int &a, int &x, int &y)
{
	const int Esq = 27; 
	int x1, y1;
	x = 0; y = 0; // ustawienie początkowej pozycji figury
	char klawisz;
	int wiersz, kolumna;
	x1 = x; y1 = y;

	do {
		system("cls"); // czyścimy ekran
		rozmiar_konsoli(kolumna, wiersz); // pobieram rozmiar konsoli
		// ograniczenie przesuwania figury do aktualnego rozmiaru konsoli
		if (a % 2 == 0)
		{
			if (x > kolumna - a / 2 - 1)
				x = kolumna - a / 2 - 1;
		}
		else
		{
			if (x > kolumna - a / 2 - 2)
				x = kolumna - a / 2 - 2;
		}

		if (x < 0)
			x = 0;

		if (y > wiersz - a)
			y = wiersz - a;

		if (y < 0)
			y = 0;
		// rysowanie figury za pomocą funkcji gotoxy
		for (int i = 1; i <= a; i++)
		{
			x1 = x + (i <= a / 2 ? i : (a - i + 1));
			y1 = y + i - 1;
			gotoxy(x1, y1);
			cout << (char)ASCII;
		}
		
		HideCursor(); 
		klawisz = _getch(); // kliknięcie klawisza powoduje "odświeżenie"

		switch (klawisz)
		{
			// powiększenie z ograniczeniem do rozmiaru konsoli.
			// przy ograniczeniu do aktualnej szerokości konsoli wielkość figury jest pomniejszona x2 (taką szerokość zajmuje w kosnoli).
			// powiększam figurę o 2 dopóki szerekość jest mniejsza od szerokości konsoli.
			// w zależności czy zadana wielkość figury jest parzysta, czy nie zmniejszam pobraną wielkość z konsoli (szerokość lub wysokość) o 1 lub 2, żeby figura nie "znikała" po zmianie rozmiarów konsoli.
		case '+': {
			if (a / 2 < (a % 2 == 0 ? kolumna - 1 : kolumna - 2) && a < (a % 2 == 0 ? wiersz : wiersz - 1)) 
				a = a + 2;
			break; }
			// zmniejszenie z ograniczeniem.
		case '-': {
			if (a > 4)
				a = a - 2;
			break; }
		case KEY_LEFT: { x--; break; }  // przesuniecie w lewo
		case KEY_RIGHT: { x++;	break; }  // przesuniecie w prawo
		case KEY_UP: { y--; break; }  // przesuniecie do gory
		case KEY_DOWN: { y++; break; }  // przesuniecie do dolu
		case Esq: { break; }       // wyjscie  
		}
	} while (klawisz != Esq);
}
//-----------------------------------------------------------------------------
void wczyt_dane(int &ASCII, int &a)
{	
	int wybor;
	cout << "ZADANIE 116 - Rysowanie figury wybranym przez użytkownika znakiem ASCII" << endl;
	cout << endl;
	cout << "|---------------|" << endl;
	cout << "|               |" << endl;
	cout << "|     MENU      |" << endl;
	cout << "|               |" << endl;
	cout << "|  1. Start     |" << endl;
	cout << "|  2. Wyjście   |" << endl;
	cout << "|               |" << endl;
	cout << "|---------------|" << endl;
	cout << endl;
	cin >> wybor;
	system("cls");

	if (wybor == 1)
	{
		cout << "Podaj kod znaku ASCII do rysowania (od 33 do 126)" << endl;
		cin >> ASCII;

		while (ASCII < 32 || ASCII > 126) // ograniczenie do przedziału widocznych znaków ASCII (kod znaków można zobaczyć na wiki)
		{
			cout << "Podaj wartość ze wskazanego przedziału (od 33 do 126)" << endl;
			cin >> ASCII;
			system("cls");
		}

		cout << "Podaj początkową wielkość figury (zalecana liczba nieparzysta): " << endl;
		cin >> a;
		cout << endl;
		system("cls");
	}
	else 
		exit(0);
	}
//-----------------------------------------------------------------------------
void rozmiar_konsoli(int &columns, int &rows)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	//printf("columns: %d\n", columns);
	//printf("rows: %d\n", rows);
}