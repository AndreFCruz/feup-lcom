#ifndef __HIGHSCORES_H
#define __HIGHSCORES_H

typedef struct {
	unsigned score;
	unsigned long minute;
	unsigned long hour;
	unsigned long day;
	unsigned long month;
	unsigned long year;
} Score_t;

Score_t * loadScore(const char* filename);

int writeScores(const char* filename, Score_t * scores);

int updateScores(Score_t* scores, Score_t newscore);

#define OK		0
#define HIGHSCORE_NUMBER		5	//Number of highest scores saved

#endif
