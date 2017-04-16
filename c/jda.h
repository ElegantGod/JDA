#pragma once


/*! \brief result */
typedef struct {
  int n; // number of faces
  int landmark_n; // number of landmarks
  int *bboxes; // bboxes of faces, (x, y, size)
  float *shapes; // shapes of faces, (x1, y1, x2, y2, ...)
  float *scores; // score of faces
} jdaResult;

/*!
 * \brief create jda cascador
 * \param model   model file
 * \return        cascador, NULL if failed
 */
void *jdaCascadorCreateDouble(const char *model);
void *jdaCascadorCreateFloat(const char *model);

/*!
 * \brief serialize model to a binary file
 * \note  this function serialze float data type, can reduce model size
 *
 * \param cascador  jda cascador
 * \param model     model file
 */
void jdaCascadorSerializeTo(void *cascador, const char *model);

/*!
 * \brief release jda cascador
 * \param cascador  jda cascador
 */
void jdaCascadorRelease(void *cascador);

/*!
 * \brief detect face
 * \param cascador  jda cascador
 * \param data      image data
 * \param width     image width
 * \param height    image height
 * \param scale     scale of image pyramid
 * \param step      sliding window step size ratio according to the window size
 * \param min_size  minimum of detection window size
 * \param max_size  maximum of detection window size, -1 for infinity
 * \param th        final score threshold, usually -0.5 ~ 0.5
 * \return          detect result
 */
jdaResult jdaDetect(void *cascador, unsigned char *data, int width, int height, \
                            float scale, float step, int min_size, int max_size, float th);

/*!
 * \brief release detection result memory
 */
void jdaResultRelease(jdaResult result);


