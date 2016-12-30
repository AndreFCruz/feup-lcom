#ifndef __HIGHSCORES_H
#define __HIGHSCORES_H

/** @defgroup Highscores Highscores
 * @{
 * Functions for manipulating the HighScores and the files interaction
 */

#define OK						0
#define HIGHSCORE_NUMBER		5	//Number of high scores saved

typedef struct {
	unsigned score;
	unsigned long minute;
	unsigned long hour;
	unsigned long day;
	unsigned long month;
	unsigned long year;
} Score_t;

/**
 * @brief Load the 5 Highest Scores from a file into an array
 *
 * @param filename path of the location of the file containing the scores
 *
 * @return Array containg the 5 Highest Scores
 */
Score_t * loadScores(const char* filename);

/**
 * @brief Writes the 5 Highest Scores from an array into a file
 *
 * @param filename path of the location of the file that is going to be written
 * @param scores Array containg the Scores that are going to be written
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int writeScores(const char* filename, Score_t * scores);

/**
 * @brief Updates the array containing the 5 Highest Scores, with a new Score.
 *
 * If the Score is not bigger than any of the Highest Scores, the array remains intact.
 *
 * @param scores Array containing the 5 Highest Scores
 * @param newscore Score to be added (or not, if not bigger)
 *
 * @return Return 0 if scores was updated, non-zero otherwise
 */
int updateScores(Score_t* scores, Score_t newscore);

#endif
