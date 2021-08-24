#ifdef ESP32

#include "nvs_flash.h"
#include "Bsp.h"

/*!*****************************************************************************
 * \fn         void Esp32Fs_init()
 * \brief      init filesystem manager
 * \param  none
 * \return     void
 *******************************************************************************/
void Esp32Fs_init()
{
    esp_err_t err = nvs_flash_init();

    if(err == ESP_ERR_NVS_NO_FREE_PAGES) {
        Debug_err("Init: %x\n", err);
        ESP_ERROR_CHECK(err);
    }
}

/*!*****************************************************************************
 * \fn         esp_err_t Esp32Fs_write(const char * filename, const char * data, const char * key)
 * \brief      write data in filename. Delete file before writing if clearFile is set.
 * \param  const char *
 * \param  const char *
 * \param  const char *
 * \return     esp_err_t
 *******************************************************************************/
esp_err_t Esp32Fs_write(const char * filename, const char * data, const char * key)
{
    nvs_handle my_handle;
    esp_err_t err;
    err = nvs_open(filename, NVS_READWRITE, &my_handle);

    if(err != ESP_OK) {
        Debug_err("Open: %x\n", err);
        return err;
    }

    err = nvs_set_str(my_handle, key, data);

    if(err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) {
        Debug_err("Write: %x\n", err);
        nvs_close(my_handle);
        return err;
    }

    err = nvs_commit(my_handle);

    if(err != ESP_OK) {
        Debug_err("Commit: %x\n", err);
        nvs_close(my_handle);
        return err;
    }

    nvs_close(my_handle);
    return err;
}

/*!*****************************************************************************
 * \fn         esp_err_t Esp32Fs_write_i16(const char * filename, const int16_t data, const char * key)
 * \brief      write data in filename. Delete file before writing if clearFile is set.
 * \param  const char *
 * \param  const char *
 * \param  const char *
 * \return     esp_err_t
 *******************************************************************************/
esp_err_t Esp32Fs_write_i16(const char * filename, const int16_t data, const char * key)
{
    nvs_handle my_handle;
    esp_err_t err;
    err = nvs_open(filename, NVS_READWRITE, &my_handle);

    if(err != ESP_OK) {
        Debug_err("Open: %x\n", err);
        return err;
    }

    err = nvs_set_i16(my_handle, key, data);

    if(err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) {
        Debug_err("Write: %x\n", err);
        nvs_close(my_handle);
        return err;
    }

    err = nvs_commit(my_handle);

    if(err != ESP_OK) {
        Debug_err("Commit: %x\n", err);
        nvs_close(my_handle);
        return err;
    }

    nvs_close(my_handle);
    return err;
}


/*!*****************************************************************************
 * \fn         char * Esp32Fs_Read(const char * filename, const char * key)
 * \brief      read allocated data.
 * \param  const char *
 * \param  const char *
 * \return     char *
 *******************************************************************************/
char * Esp32Fs_read(const char * filename, const char * key)
{
    nvs_handle my_handle;
    esp_err_t err;
    err = nvs_open(filename, NVS_READWRITE, &my_handle);

    if(err != ESP_OK) {
        Debug_err("Open: %x\n", err);
        return NULL;
    }

    size_t required_size;
    err = nvs_get_str(my_handle, key, NULL, &required_size);

    if(err != ESP_OK) {
        Debug_info("File NOT found or wrong size: %x\n", err);
        nvs_close(my_handle);
        return NULL;
    }

    char * data = (char *) OsMalloc(required_size * sizeof(char));

    if(data == NULL) {
        Debug_err("Malloc\n");
        return NULL;
    }

    err = nvs_get_str(my_handle, key, data, &required_size);

    if(err != ESP_OK) {
        Debug_info("Read data: %x\n", err);
        nvs_close(my_handle);
        OsFree(data);
        return NULL;
    }

    nvs_close(my_handle);
    return data;
}

/*!*****************************************************************************
 * \fn         char * Esp32Fs_read_i16(const char * filename, const char * key)
 * \brief      read allocated data.
 * \param  const char *
 * \param  const char *
 * \return     char *
 *******************************************************************************/
int16_t Esp32Fs_read_i16(const char * filename, const char * key)
{
    nvs_handle my_handle;
    esp_err_t err;
    err = nvs_open(filename, NVS_READWRITE, &my_handle);

    if(err != ESP_OK) {
        Debug_err("Open: %x\n", err);
        return NULL;
    }

    int16_t valOut;
    err = nvs_get_i16(my_handle, key, &valOut);

    if(err != ESP_OK) {
        Debug_info("Data NOT found: %x\n", err);
        nvs_close(my_handle);
        return 0;
    }

    nvs_close(my_handle);
    return valOut;
}
/*!*****************************************************************************
 * \fn         int Esp32Fs_exists(const char * filename)
 * \brief      check that filename exists.
 * \param  const char *
 * \return     int : 0 if not present, 1 if present
 *******************************************************************************/
int Esp32Fs_exists(const char * filename)
{
    nvs_handle my_handle;
    esp_err_t err;
    err = nvs_open(filename, NVS_READONLY, &my_handle);

    if(err != ESP_OK) {
        return 0;
    }

    nvs_close(my_handle);
    return 1;
}

/*!*****************************************************************************
 * \fn         int Esp32Fs_GetLength(const char * filename)
 * \brief      return filename length
 * \param  const char *
 * \param  const char *
 * \return     int
 *******************************************************************************/
int Esp32Fs_getLength(const char * filename, const char * key)
{
    nvs_handle my_handle;

    if(nvs_open(filename, NVS_READONLY, &my_handle) != ESP_OK)
        return ESP_FAIL;

    size_t required_size;

    if(nvs_get_str(my_handle, key, NULL, &required_size) != ESP_OK) {
        Debug_err("File NOT found or wrong size.\n");
        nvs_close(my_handle);
        return ESP_FAIL;
    }

    nvs_close(my_handle);
    return (int)required_size;
}

/*!*****************************************************************************
 * \fn         void Esp32Fs_cleanup(void)
 * \brief      cleanup filesystem manager
 * \param  none
 * \return     void
 *******************************************************************************/
void Esp32Fs_cleanup()
{
}

/*!*****************************************************************************
 * \fn         int Esp32Fs_delete(const char * filename, const char * key)
 * \brief      delete filename key
 * \param  const char *
 * \param  const char *
 * \return     int
 *******************************************************************************/
int Esp32Fs_delete(const char * filename, const char * key)
{
    nvs_handle my_handle;

    if(nvs_open(filename, NVS_READWRITE, &my_handle) != ESP_OK) {
        Debug_err("Open\n");
        return ESP_FAIL;
    }

    return nvs_erase_key(my_handle, key);
}

/*!*****************************************************************************
 * \fn         int Esp32Fs_delete(const char * filename)
 * \brief      delete filename
 * \param  const char *
 * \param  const char *
 * \return     int
 *******************************************************************************/
int Esp32Fs_delete_all(const char * filename)
{
    nvs_handle my_handle;

    if(nvs_open(filename, NVS_READWRITE, &my_handle) != ESP_OK) {
        Debug_err("Open\n");
        return ESP_FAIL;
    }

    return nvs_erase_all(my_handle);
}

#endif // ESP32
