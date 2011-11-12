#include "fileIO.h"

unsigned char *mapToFile;
// creating a mmap for writing to a file at the end
int loadMMapForFile(unsigned char fileName[]){

    int fd;
    int result;
    off_t fileSize = FILE_SIZE*1024*1024;
    //unsigned char *map;  /* mmapped array of int's */

    /* Open a file for writing.
     *  - Creating the file if it doesn't exist.
     *  - Truncating it to 0 size if it already exists. (not really needed)
     *
     * Note: "O_WRONLY" mode is not sufficient when mmaping.
     */
    fd = open((char *)fileName, O_RDWR | O_CREAT | O_TRUNC, (mode_t)0644);
    if (fd == -1) {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);
    }

    /* Stretch the file size to the size of the (mmapped) array of ints
     */
    result = lseek(fd, fileSize-1, SEEK_SET);
    if (result == -1) {
        close(fd);
        perror("Error calling lseek() to 'stretch' the file");
        exit(EXIT_FAILURE);
    }

    /* Something needs to be written at the end of the file to
     * have the file actually have the new size.
     * Just writing an empty string at the current file position will do.
     *
     * Note:
     *  - The current position in the file is at the end of the stretched 
     *    file due to the call to lseek().
     *  - An empty string is actually a single '\0' character, so a zero-byte
     *    will be written at the last byte of the file.
     */
    result = write(fd, "", 1);
    if (result != 1) {
        close(fd);
        perror("Error writing last byte of the file");
        exit(EXIT_FAILURE);
    }

    /* Now the file is ready to be mmapped.
     */
    mapToFile = (unsigned char *)mmap(0, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mapToFile == MAP_FAILED) {
        close(fd);
        perror("Error mmapping the file");
        exit(EXIT_FAILURE);
    }

    return fd;
}
