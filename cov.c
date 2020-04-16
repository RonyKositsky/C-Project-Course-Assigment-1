#include <stdio.h>
#include<stdlib.h>
#include <assert.h>

FILE* input_file;
FILE* output_file;
double** Matrix;
int rows = 0;
int colums = 0;
void CopyValues(FILE* input_file);

void GenerateMatrix(FILE* file,int rows) {
	int i;
	Matrix = (double**)calloc(rows, sizeof(double*));
	for (i = 0; i < rows; i++) {
		Matrix[i] = (double*)calloc(rows, sizeof(double));
	}

	CopyValues(file);
	fclose(file);
}

void CopyValues(FILE* input_file) {
	int i, j;

	for (i = 0; i < rows; i++) {
		for (j = 0; j < colums; j++) {
			fread(&Matrix[i][j], sizeof(double), 1, input_file);
		}
	}
}

double mean(double *arr, int n)
{
	int i;
	double sum = 0;
	for (i = 0; i < n; i++)
		sum = sum + arr[i];
	return sum / n;
}

double covariance(double *arr1, double *arr2, int n)
{
	int i;
	double mean1,mean2,sum = 0;
	mean1 = mean(arr1, n);
	mean2 = mean(arr2, n);
	for (i = 0; i < n; i++)
		sum = sum + (arr1[i] - mean1) *
		(arr2[i] - mean2);
	return sum;
}

void WriteToFile(char* fileName) {
	int i, j;
	double* CovRoW;
	
	output_file = fopen(fileName, "w");
	rewind(output_file);

	fwrite(&rows, sizeof(int), 1, output_file);
	fwrite(&rows, sizeof(int), 1, output_file);

	CovRoW = calloc(sizeof(int), rows);

	for (i = 0; i < rows; i++) {
		for (j = 0; j < rows; j++) {
			CovRoW[j] = covariance(Matrix[i], Matrix[j], colums);
		}

		fwrite(CovRoW, sizeof(double), rows, output_file);
	}

	fclose(output_file);
}

int main(int argc, char* argv[]) {
	assert(argc != -1);
	input_file= fopen(argv[1], "r");

	fread(&colums, sizeof(int), 1, input_file);
	fread(&rows, sizeof(int), 1, input_file);

	GenerateMatrix(input_file,rows);
	WriteToFile(argv[2]);
	return 0;
}
