#include <dos.h>
#include <stdio.h>

struct VIDEO
{
	unsigned char symb;
	unsigned char attr;
};

void get_reg();
unsigned char col=0x5E;

void interrupt(*old08) (...);
void interrupt(*old09) (...);
void interrupt(*old0A) (...);
void interrupt(*old0B) (...);
void interrupt(*old0C) (...);
void interrupt(*old0D) (...);
void interrupt(*old0E) (...);
void interrupt(*old0F) (...);

void interrupt(*old70) (...);
void interrupt(*old71) (...);
void interrupt(*old72) (...);
void interrupt(*old73) (...);
void interrupt(*old74) (...);
void interrupt(*old75) (...);
void interrupt(*old76) (...);
void interrupt(*old77) (...);

void interrupt new08(...);
void interrupt new09(...);
void interrupt new0A(...);
void interrupt new0B(...);
void interrupt new0C(...);
void interrupt new0D(...);
void interrupt new0E(...);
void interrupt new0F(...);

void interrupt new70(...);
void interrupt new71(...);
void interrupt new72(...);
void interrupt new73(...);
void interrupt new74(...);
void interrupt new75(...);
void interrupt new76(...);
void interrupt new77(...);

void print(int offset, int val)
{
	VIDEO far* screen = (VIDEO far*) MK_FP(0xB800, 0);
	screen += offset;
	for (int i = 7; i >= 0; --i)
	{
		screen->symb = val % 2 + '0';
		screen->attr = col;
		screen++;
		val /= 2;
	}
}

void get_reg()
{
	print(0, inp(0x21));

	outp(0x20, 0x0A);
	print(32, inp(0x20));

	outp(0x20, 0x0B);
	print(16, inp(0x20));

	print(48, inp(0xA1));

	outp(0xA0, 0x0A);
	print(80, inp(0xA0));

	outp(0xA0, 0x0B);
	print(64, inp(0xA0));
}

void interrupt new08(...)
{
	get_reg();
	old08();
}

void interrupt new09(...)
{
	get_reg();
	col += 0x06;
	get_reg();
	old09();
}

void interrupt new0A(...)
{
	get_reg();
	old0A();
}

void interrupt new0B(...)
{
	get_reg();
	old0B();
}

void interrupt new0C(...)
{
	get_reg();
	old0C();
}

void interrupt new0D(...)
{
	get_reg();
	old0D();
}

void interrupt new0E(...)
{
	get_reg();
	old0E();
}

void interrupt new0F(...)
{
	get_reg();
	old0F();
}

void interrupt new70(...)
{
	get_reg();
	old70();
}

void interrupt new71(...)
{
	get_reg();
	old71();
}

void interrupt new72(...)
{
	get_reg();
	old72();
}

void interrupt new73(...)
{
	get_reg();
	old73();
}

void interrupt new74(...)
{
	get_reg();
	col += 0x06;
	get_reg();
	old74();
}

void interrupt new75(...)
{
	get_reg();
	old75();
}

void interrupt new76(...)
{
	get_reg();
	old76();
}

void interrupt new77(...)
{
	get_reg();
	old77();
}

void init()
{
	old08 = getvect(0x08);
	old09 = getvect(0x09);
	old0A = getvect(0x0A);
	old0B = getvect(0x0B);
	old0C = getvect(0x0C);
	old0D = getvect(0x0D);
	old0E = getvect(0x0E);
	old0F = getvect(0x0F);

	setvect(0x08, new08);
	setvect(0x09, new09);
	setvect(0x0A, new0A);
	setvect(0x0B, new0B);
	setvect(0x0C, new0C);
	setvect(0x0D, new0D);
	setvect(0x0E, new0E);
	setvect(0x0F, new0F);

	old70 = getvect(0x70);
	old71 = getvect(0x71);
	old72 = getvect(0x72);
	old73 = getvect(0x73);
	old74 = getvect(0x74);
	old75 = getvect(0x75);
	old76 = getvect(0x76);
	old77 = getvect(0x77);

	setvect(0xC8, new70);
	setvect(0xC9, new71);
	setvect(0xCA, new72);
	setvect(0xCB, new73);
	setvect(0xCC, new74);
	setvect(0xCD, new75);
	setvect(0xCE, new76);
	setvect(0xCF, new77);

	_disable();

	outp(0x20, 0x11);
	outp(0x21, 0x08);
	outp(0x21, 0x04);
	outp(0x21, 0x01);

	outp(0xA0, 0x11);
	outp(0xA1, 0xC8);
	outp(0xA1, 0x02);
	outp(0xA1, 0x01);

	_enable();
}

int main()
{
	unsigned far *fp;
	init();

	FP_SEG(fp) = _psp;
	FP_OFF(fp) = 0x2c;

	_dos_freemem(*fp);

	_dos_keep(0, (_DS - _CS) + (_SP / 16) + 1);

	return 0;
}







