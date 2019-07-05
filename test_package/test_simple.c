/*
 * Tests for minizip Conan package
 * Pack/unpack one file
 * Dmitriy Vetutnev, Odant 2019
*/


#include <mz.h>
#include <mz_strm.h>
#include <mz_zip.h>
#include <mz_zip_rw.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


const char text[] = ""
"So feel been kept be at gate. Be september it extensive oh concluded of certainty. In read most gate at body held it ever no. Talking justice welcome message inquiry in started of am me. Led own hearted highest visited lasting sir through compass his. Guest tiled he quick by so these trees am. It announcing alteration at surrounded comparison.\n"
"Maids table how learn drift but purse stand yet set. Music me house could among oh as their. Piqued our sister shy nature almost his wicket. Hand dear so we hour to. He we be hastily offence effects he service. Sympathize it projection ye insipidity celebrated my pianoforte indulgence. Point his truth put style. Elegance exercise as laughing proposal mistaken if. We up precaution an it solicitude acceptance invitation.\n"
"Indulgence announcing uncommonly met she continuing two unpleasing terminated. Now busy say down the shed eyes roof paid her. Of shameless collected suspicion existence in. Share walls stuff think but the arise guest. Course suffer to do he sussex it window advice. Yet matter enable misery end extent common men should. Her indulgence but assistance favourable cultivated everything collecting.\n"
"It if sometimes furnished unwilling as additions so. Blessing resolved peculiar fat graceful ham. Sussex on at really ladies in as elinor. Sir sex opinions age properly extended. Advice branch vanity or do thirty living. Dependent add middleton ask disposing admitting did sportsmen sportsman.\n"
"Marianne or husbands if at stronger ye. Considered is as middletons uncommonly. Promotion perfectly ye consisted so. His chatty dining for effect ladies active. Equally journey wishing not several behaved chapter she two sir. Deficient procuring favourite extensive you two. Yet diminution she impossible understood age.\n"
"Civility vicinity graceful is it at. Improve up at to on mention perhaps raising. Way building not get formerly her peculiar. Up uncommonly prosperous sentiments simplicity acceptance to so. Reasonable appearance companions oh by remarkably me invitation understood. Pursuit elderly ask perhaps all.\n"
"Chapter too parties its letters nor. Cheerful but whatever ladyship disposed yet judgment. Lasted answer oppose to ye months no esteem. Branched is on an ecstatic directly it. Put off continue you denoting returned juvenile. Looked person sister result mr to. Replied demands charmed do viewing ye colonel to so. Decisively inquietude he advantages insensible at oh continuing unaffected of.\n"
"Be me shall purse my ought times. Joy years doors all would again rooms these. Solicitude announcing as to sufficient my. No my reached suppose proceed pressed perhaps he. Eagerness it delighted pronounce repulsive furniture no. Excuse few the remain highly feebly add people manner say. It high at my mind by roof. No wonder worthy in dinner.\n"
"To sorry world an at do spoil along. Incommode he depending do frankness remainder to. Edward day almost active him friend thirty piqued. People as period twenty my extent as. Set was better abroad ham plenty secure had horses. Admiration has sir decisively excellence say everything inhabiting acceptance. Sooner settle add put you sudden him.\n"
"Talking chamber as shewing an it minutes. Trees fully of blind do. Exquisite favourite at do extensive listening. Improve up musical welcome he. Gay attended vicinity prepared now diverted. Esteems it ye sending reached as. Longer lively her design settle tastes advice mrs off who.\n"
;


int main(void) {
    FILE* fp = NULL;

    fp = fopen("simple.txt", "wb");
    if (fp == NULL) {
        printf("Can`t open 'simple.txt' for write!\n");
        return EXIT_FAILURE;
    }
    fwrite(text, sizeof(char), sizeof(text) - 1, fp);
    fclose(fp);

    void* writer = NULL;
    int32_t err = MZ_OK;

    mz_zip_writer_create(&writer);
    mz_zip_writer_set_compress_method(writer, MZ_COMPRESS_METHOD_DEFLATE);
    mz_zip_writer_set_compress_level(writer, MZ_COMPRESS_LEVEL_BEST);
    err = mz_zip_writer_open_file(writer, "simple.zip", 0, 0);
    if (err != MZ_OK) {
        printf("Failed 'mz_zip_writer_open_file()', err: %d\n", err);
        return EXIT_FAILURE;
    }
    err = mz_zip_writer_add_path(writer, "simple.txt", NULL, 0, 1);
    if (err != MZ_OK) {
        printf("Failed 'mz_zip_writer_add_path()', err: %d\n", err);
        return EXIT_FAILURE;
    }
    err = mz_zip_writer_close(writer);
    if (err != MZ_OK) {
        printf("Failed 'mz_zip_writer_close()', err: %d\n", err);
        return EXIT_FAILURE;
    }
    mz_zip_writer_delete(&writer);

    printf("Pack OK, unpack...\n");
    remove("simple.txt");

    void* reader = NULL;

    mz_zip_reader_create(&reader);
    err = mz_zip_reader_open_file(reader, "simple.zip");
    if (err != MZ_OK) {
        printf("Failed 'mz_zip_reader_open_file()', err: %d\n", err);
        return EXIT_FAILURE;
    }
    err = mz_zip_reader_save_all(reader, ".");
    if (err != MZ_OK) {
        printf("Failed 'mz_zip_reader_save_all()', err: %d\n", err);
        return EXIT_FAILURE;
    }
    err = mz_zip_reader_close(reader);
    if (err != MZ_OK) {
        printf("Failed 'mz_zip_reader_close()', err: %d\n", err);
        return EXIT_FAILURE;
    }
    mz_zip_reader_delete(&reader);

    printf("Unpack OK, check content...\n");

    size_t file_size = 0;
    char* buffer = NULL;

    fp = fopen("simple.txt", "rb");
    if (fp == NULL) {
        printf("Can`t open 'simple.txt' for read!\n");
        return EXIT_FAILURE;
    }
    fseek(fp, 0L, SEEK_END);
    file_size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    buffer = malloc(file_size + 1);
    fread(buffer, sizeof(char), file_size, fp);
    buffer[file_size] = 0;
    fclose(fp);
    if (strcmp(text, buffer) != 0) {
        printf("Content not equal!\n");
        return EXIT_FAILURE;
    }
    free(buffer);

    printf("Content equal, OK\n");
    return EXIT_SUCCESS;
}