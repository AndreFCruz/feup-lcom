#include <stdio.h>
#include <stdlib.h>
#include "Highscores.h"
#include "GVector.h"

Score_t * loadScores(const char* filename) {
    Score_t scores[HIGHSCORE_NUMBER];

	// allocating necessary size
    //Score_t* bmp = (Bitmap*) malloc(sizeof(Bitmap));

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
    	fscanf(filePtr,"%u",& scores[i].score);
		fseek(filePtr,2,SEEK_CUR);					//Ignoring ", "

		fscanf(filePtr,"%u",& scores[i].hour);
		fseek(filePtr,1,SEEK_CUR);					//Ignoring ":"

		fscanf(filePtr,"%u",& scores[i].minute);
		fseek(filePtr,1,SEEK_CUR);					//Ignoring " "

		fscanf(filePtr,"%u",& scores[i].day);
		fseek(filePtr,1,SEEK_CUR);					//Ignoring "/"

		fscanf(filePtr,"%u",& scores[i].month);
		fseek(filePtr,1,SEEK_CUR);					//Ignoring "/"

		fscanf(filePtr,"%u",& scores[i].year);
		fseek(filePtr,1,SEEK_CUR);					//Ignoring \n
    }

    fclose(filePtr);
    return scores;
}

int writeScores(const char* filename, Score_t * scores) {
	// open filename
    FILE *filePtr;
    filePtr = fopen(filename, "w");
    if (filePtr == NULL) {
    	printf("writeScores -> File Non Existent.\n");
    	return 1;
    }

    unsigned i;
    for (i = 0; i < HIGHSCORE_NUMBER; ++i) {
    	fprintf(filePtr, "%u, %u:%u %u/%u/%u\n", scores[i].score, scores[i].hour,
    			scores[i].minute, scores[i].day, scores[i].month, scores[i].year);
    }
}

int updateScores(Score_t* scores, Score_t newscore) {
	//Flag at OK if updated, else at 1
	int updated = 1;
	unsigned i;

	for (i = 0; i < HIGHSCORE_NUMBER; ++i) {
		if (scores[i].score < newscore.score) {
			Score_t helper = scores[i];

			scores[i] = newscore;	//Allowed
			newscore = helper;
			updated = OK;
		}
	}

	if (updated)
		return OK;
	else return 1;

}
