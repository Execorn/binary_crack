/*
#include <stdio.h>

int main()
{
	FILE *new_cracked_input = fopen("crack_overflow.txt", "wb");
	char pass_buffer[222] = "";

	for (int i = 0; i < 222; ++i) {
		pass_buffer[i] = '\0';
	}

	pass_buffer[200] = 0x33;

	fwrite(pass_buffer, 222, sizeof (char), new_cracked_input);
	fclose(new_cracked_input);

	return 0;
}
*/