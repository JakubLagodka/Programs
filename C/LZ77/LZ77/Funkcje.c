
#include <stdio.h>
#include "Funkcje.h"



void Kompresja(char *dane, int rozmiar, FILE *out, char *slownik_char, char *bufor_char)
{
	int rozmiar_slownika, rozmiar_bufora;
	rozmiar_slownika = pow(2, atoi(slownik_char));
	rozmiar_bufora = pow(2, atoi(bufor_char));

	char znak;
	char znak_bufora, znak_slownika, *pomoc, *okno2;
	int i,licznik = 0, j, pozycja, max_dlugosc = 0, przesuniecie = 0, bufor, czy_przekroczenie = 0;
	
	
	okno2 = malloc(sizeof(char)*(rozmiar_bufora + rozmiar_slownika + rozmiar + 1));		//alokuje pami��	
	pomoc = malloc(sizeof(char)*(rozmiar_bufora + rozmiar_slownika + 1));		//alokuje pami��	
	okno2[0] = 0;	




	for (j = 0; j < rozmiar_slownika; j++)
		okno2[j] = dane[0];
	for (j = 0; j < rozmiar_bufora; j++)
		okno2[rozmiar_slownika +j] = dane[j];
	
	fwrite(dane, sizeof(char), 1, out);		//wyprowadzam do pliku pierwszy znak
	przesuniecie = rozmiar_slownika;
	while (przesuniecie < rozmiar)	// pracuj� w p�tli, kt�rej ilo�� wywo�a� jest zale�na od d�_s�ownika i rozmiaru
	{
		
		max_dlugosc = 0;	//deklaruje zmienne pomocnicze
		pozycja = 0;
		
		bufor = przesuniecie + rozmiar_slownika;
		znak_bufora = okno2[rozmiar_slownika];
		
		for (j = 0; j < rozmiar_bufora + 2; j++)	//w kolejnej p�tli sprawdzam znak po znaku czy znaki 
		{													//si� powtarzaj�
			znak_slownika = okno2[j];	//kopiuje ze s�ownika i bufora znaki do zmiennych pomocniczych by �atwiej
										//na nich operowa�
			if (licznik != 0)			//Je�eli pierwszy znak si� pokrywa, to sprawdzam ile kolejnych znak�w
			{							//si� pokrywa
				znak_bufora = okno2[licznik + rozmiar_slownika + licznik];

				if (znak_bufora == znak_slownika && licznik < rozmiar_slownika - 1 && j < rozmiar_slownika + 1)
					licznik++;	//j�sli znaki s� r�wne i nie przekroczono rozmiaru s�ownika

				else		//je�li znaki s� r�zne, to zapisuje pozycje i d�ugo�� tych, co si� pokrywa�y
				{
					if (licznik > max_dlugosc)	//w sytuacji gdy ci�g by� wi�kszy ni� poprzednie w s�owniku
					{
						max_dlugosc = licznik;	//to zapami�tuje parametry niezb�dne do konwersji
						pozycja = j - licznik;

						znak = okno2[licznik + rozmiar_slownika];
					}
					licznik = 0;	//zeruje licznik
					znak_bufora = okno2[rozmiar_slownika];
				}
			}
			if (licznik == 0 && j >= rozmiar_slownika)	//je�eli sprawdzi�em ju� dane w s�owniku, to wychodz� z p�tli
				break;
			if (licznik == 0 && znak_bufora == znak_slownika)
				licznik++;		//je�eli znak z bufora si� pokrywa ze znakiem ze s�ownika zwi�kszam licznik 
		}
		if (max_dlugosc == 0)	//jesli w s�owniku w og�le nie znalaz�em takich samych znak�w
			znak = okno2[rozmiar_slownika];

		if (przesuniecie == rozmiar_slownika)
			przesuniecie++;

		for (i = 0; i < rozmiar_bufora + rozmiar_slownika - max_dlugosc - 1; i++)
			okno2[i] = okno2[max_dlugosc + 1 + i];
		i = 0;
		for (j = rozmiar_bufora + rozmiar_slownika - max_dlugosc - 1; j < rozmiar_bufora + rozmiar_slownika; j++, i++)
			okno2[j] = dane[przesuniecie + i - 1];
		
		
		przesuniecie += i;	//zapisuje przesuni�cie okna
		
		
		sprintf(pomoc, "%d", pozycja);
		fwrite(pomoc, sizeof(char), strlen(pomoc), out);		//wyprowadzam do pliku pierwszy znak
		if (rozmiar_slownika > 9)
		fwrite(",", sizeof(char), 1, out);		//wyprowadzam do pliku pierwszy znak
		sprintf(pomoc, "%d", max_dlugosc);
		fwrite(pomoc, sizeof(char), strlen(pomoc), out);		//wyprowadzam do pliku pierwszy znak
		if (rozmiar_slownika > 9)
		fwrite(",", sizeof(char), 1, out);		//wyprowadzam do pliku pierwszy znak
		
		fwrite(&znak, sizeof(char), 1, out);		//wyprowadzam do pliku pierwszy znak
		

	}
	
}

void Dekompresja(char *wejscie, int rozmiar, FILE *out, char *slownik_char, char *bufor_char)
{
	int rozmiar_slownika, rozmiar_bufora;
	rozmiar_slownika = pow(2, atoi(slownik_char));
	rozmiar_bufora = pow(2, atoi(bufor_char));

	char *okno;
	okno = malloc(sizeof(char)*(rozmiar_bufora + rozmiar_slownika + rozmiar + 1));		//alokuje pami��
	okno[0] = 0;

	int i, pozycja, dlugosc;	//deklaruje zmienne pomocnicze
	wchar_t znak;
	
	for (i = 0; i < rozmiar_slownika; i++)
		okno[i] = wejscie[i];
	
	for (i = 1; i < rozmiar; i++)	//w p�tli analizuje znak po znaku dane wej�ciowe
	{
			pozycja = wejscie[i] - '0';

			while (rozmiar_slownika > 9 && wejscie[i + 1] != ',')
			{
				pozycja *= 10;
				i++;
				pozycja += wejscie[i] - '0';
			}
			i++;
		
			dlugosc = wejscie[i] - '0';

			while (rozmiar_slownika > 9 && wejscie[i + 1] != ',')
			{
				dlugosc *= 10;
				i++;
				dlugosc += wejscie[i] - '0';
			}
			i++;

			for (int j = 0; j < dlugosc; j++)
				okno[rozmiar_slownika + j] = okno[pozycja + j];
			
			okno[rozmiar_slownika + dlugosc] = wejscie[i];
			
			fwrite(&okno[rozmiar_slownika], sizeof(char), dlugosc + 1, out);

			for (int j = 0; j < rozmiar_slownika; j++)
				okno[j] = okno[dlugosc + 1 + j];
			
				dlugosc = 0;
	}
}
	
		
