#include <stdio.h>
#include <stdlib.h>
#include "Highscores.h"

Score_t * loadScores(const char* filename) {
	//Score_t scores[HIGHSCORE_NUMBER];
	Score_t * scores = (Score_t*) malloc(sizeof(Score_t) * HIGHSCORE_NUMBER);

	// open filename
	FILE *filePtr;
	filePtr = fopen(filename, "r");
	if (filePtr == NULL) {
		printf("loadScores -> File Non Existent.\n");
		return NULL;
	}

	unsigned i;
	for (i = 0; i < HIGHSCORE_NUMBER; ++i) {
		//Reading "score, hour:minute day/month/year"
		fscanf(filePtr, "%u", &scores[i].score);
		fseek(filePtr, 2, SEEK_CUR);					//Ignoring ", "

		fscanf(filePtr, "%u", &scores[i].hour);
		fseek(filePtr, 1, SEEK_CUR);					//Ignoring ":"

		fscanf(filePtr, "%u", &scores[i].minute);
		fseek(filePtr, 1, SEEK_CUR);					//Ignoring " "

		fscanf(filePtr, "%u", &scores[i].day);
		fseek(filePtr, 1, SEEK_CUR);					//Ignoring "/"

		fscanf(filePtr, "%u", &scores[i].month);
		fseek(filePtr, 1, SEEK_CUR);					//Ignoring "/"

		fscanf(filePtr, "%u", &scores[i].year);
		fseek(filePtr, 1, SEEK_CUR);					//Ignoring \n
	}

	fclose(filePtr);
	return scores;
}

int writeScores(const char* filename, Score_t* scores) {
	printf("\t\tWRITE SCORES CALLED!\n");
	// open filename
	FILE* filePtr;
	filePtr = fopen(filename, "w");
	if (filePtr == NULL) {
		printf("writeScores -> File Non Existent.\n");
		return 1;
	}

	unsigned i;
	for (i = 0; i < HIGHSCORE_NUMBER; ++i) {
		fprintf(filePtr, "%u, %u:%u %u/%u/%u\n", scores[i].score,
				scores[i].hour, scores[i].minute, scores[i].day,
				scores[i].month, scores[i].year);
		printf("writing score: %u, %u:%u %u/%u/%u\n", scores[i].score,
				scores[i].hour, scores[i].minute, scores[i].day,
				scores[i].month, scores[i].year);
	}
	fclose(filePtr);

	return OK;
}

int updateScores(Score_t* scores, Score_t newscore) {
	printf("\t\tUPDATE SCORES CALLED\n");
	int updated = 0; // updated flag
	unsigned i;

	for (i = 0; i < HIGHSCORE_NUMBER; ++i) {
		if (scores[i].score < newscore.score) {
			Score_t helper = scores[i];

			scores[i] = newscore;
			newscore = helper;
			updated = 1;
			printf("updating score: %u, %u:%u %u/%u/%u\n", scores[i].score,
					scores[i].hour, scores[i].minute, scores[i].day,
					scores[i].month, scores[i].year);
		}
	}

	return updated;

}
