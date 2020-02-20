/** @file bio.h 
 * FingerPrint APIs and Definitions
 *
 * Company Name: Linkwell Telesystems Pvt. Ltd (visiontek.co.in)
 * 
 * API Description: FP APIs.  
 */

/**
 * 
 * @file bio.h 
 */
#define BIO_SUPREMA_IN_ISO_FORMAT 1

// conversion types
#define RAW2WSQ			1
#define WSQ2RAW			2
#define ISO2BIR			3
#define BIR2ISO			4

/** @brief
 *      Initializes the fingerprint api library.
 * @param[in] sensor_settings Default 0
 * @return 
 *       0 on succes , 
 *       -1 on failure.
 * @note
 * 	Default initialisation gives ISO template as output. For other format, use bio_init_TF() API.
 */
int bio_init(int sensor_settings) ;

/** @brief
 * 	Initializes the fingerprint api library with specified template format.
 *  @param[in] format Format in which templates to be saved.\n
 *  			ID_ISO_PK_DATA_ISO_FMR,\n
 *  			ID_ISO_PK_DATA_MINEX_A,\n
 *  			ID_ISO_PK_DATA_ANSI_378,\n
 *  			ID_PKCOMP	are the supported template formats.
 *  @return
 *  	0 on success ,
 *  	-1 on failure.
 */
int bio_init_TF(int format);

/** @brief
 *      unInitializes the fingerprint api library       
 * @param[in] fd return of bio_init.
 * @return 
 *      void.
 */
void bio_uninit(int fd) ;

/** @brief
 *      scans the finger and add to the module          
 * @param[in] fd Return of bio_init.
 * @param[out] userid On return contain userid for a enrolled finger.
 * @param[in] sensor_timeout_period Timeout in seconds.
 * @param[out] quality Fingerprint image quality.
 * @return 
 *      0 on success,
 *      -1 on failure,
 */
int bio_enroll_by_scan(int fd, unsigned long *userid, int sensor_timeout_period, int quality) ;

/**@brief
 *      Add the provided image data to Database.     
 * @param[in] fd Return of bio_init.
 * @param[out] userid On return contain userid for a enrolled finger.
 * @param[in] image_data finger raw image data.
 * @param[in] image_len image data length.
 * @param[out] quality Fingerprint image quality.
 * @return
 *      0 on success,
 *      -1 on failure.
 * 
 * @note
 *      This API is not applicable for Optical scanners.
 */
int bio_enroll_by_image(int fd, unsigned long *userid, unsigned char *image_data, int image_len, int quality) ;

/**@brief
 *      Add the finger data to database.
 * @param[in] fd Return of bio_init.
 * @param[out] userid On return contain userid for a enrolled finger.
 * @param[in] template_data Template data.
 * @param[in] template_len Length of a template data.
 * @return
 *      0 on success,
 *      -1 on failure.
 */
int bio_enroll_by_template(int fd, unsigned long *userid, unsigned char *template_data, int template_len);

/**@brief
 *      Reads the last scan image to image_file_name.
 * @param[in] fd Return of bio_init.
 * @param[in] image_file_name Name of the image file to be saved as.
 * @param[in] file_type Always 0
 * @return
 *      0 on success,
 *      -1 on failure.
 */
int bio_read_last_scanned_image(int fd, char *image_file_name, short int file_type) ;

/**@brief
 *      Read the last scan template.
 * @param[in] fd Return of bio_init.
 * @param[in] template_file_name Name of the template file to be saved as.
 * @return
 *      0 on success,
 *      -1 on failure.
 */
int bio_read_last_scanned_template(int fd, char *template_file_name) ;

/**@brief
 *      Store the template into file retrieve from the module.
 * @param[in] fd Return of bio_init.
 * @param[in] userid userid of the template to read
 * @param[in] template_file_name Name of the template file to be saved as.
 * @return
 *      0 on success,
 *      -1 on failure.
 * @note
 *      This API is not applicable for Optical scanners.
 */
int bio_read_template(int fd, unsigned long *userid, char *template_file_name) ;

/**@brief
 *      Store the template into specified file by live scan.
 * @param[in] fd Return of bio_init.
 * @param[in] template_file_name Name of the template to be saved with.
 * @param[in]  sensor_timeout_period Timeout in seconds.
 * @param[out] quality Fingerprint image quality.
 * @return
 *      0 on success,
 *      -1 on failure.
 */
int bio_scan_template(int fd, char *template_file_name, int sensor_timeout_period, int quality) ;

/**@brief
 *      Store the image into specified file by live scan.
 * @param[in] fd Return of bio_init.
 * @param[in] image_file_name Name of the Image to be saved with.
 * @param[in] file_type 0 for raw image.
 * @param[in] sensor_timeout_period Timeout in seconds.
 * @param[out] quality Fingerprint image quality.
 * @return
 *      0 on success,
 *      -1 on failure.
 */
int bio_scan_image(int fd, char *image_file_name, short int file_type, int sensor_timeout_period, int quality) ;

/**@brief
 *      Store the gray scale image into file by live scan.      
 * @param[in] fd Return of bio_init.
 * @param[in] image_file_name Name of the Image to be saved with.
 * @param[in] file_type 0 for raw image.
 * @param[in] sensor_timeout_period Timeout in seconds.
 * @param[out] quality Fingerprint image quality.
 * @return
 *      0 on success,
 *      -1 on failure.
 */
int bio_scan_image_gray(int fd, char *image_file_name, short int file_type, int sensor_timeout_period, int quality); 

/**@brief
 *      Store the template and image into files by live scan    
 * @param[in] fd Return of bio_init.
 * @param[in] template_file_name Name of the template to be saved with.
 * @param[in] image_file_name Name of the Image to be saved with.
 * @param[in] sensor_timeout_period Timeout in seconds.
 * @param[out] quality Fingerprint image quality.
 * @return
 *      0 on success,
 *      -1 on failure.
 */
int bio_scan_template_and_image(int fd,char *template_file_name,char *image_file_name,int sensor_timeout_period,int quality) ;

/**@brief
 *      Store the template and image into files by live scan with provided format.
 * @param[in] fd Return of bio_init.
 * @param[in] template_file_name Name of the template to be saved with.
 * @param[in] image_file_name Name of the Image to be saved with.
 * @param[in] sensor_timeout_period Timeout in seconds.
 * @param[out] quality Fingerprint image quality.
 * @param[in] format Format of the template.
 * @return
 *      0 on success,
 *      -1 on failure.
 */
int bio_scan_template_and_image_by_format(int fd, char *template_file_name, char *image_file_name, int sensor_timeout_period, int quality,int format);

/**@brief
 *      verify the finger by scan aganist module.       
 * @param[in] fd Return of bio_init.
 * @param[in] userid Userid which you want to verify.
 * @param[in] sensor_timeout_period Timeout in seconds.
 * @return
 *      0 on success,
 *      -1 on failure.
 */
int bio_verify_by_scan(int fd, unsigned long *userid, int sensor_timeout_period) ;

/**@brief
 *      verifies userid against the template sent from the Host 
 * @param[in] fd Return of bio_init.
 * @param[in] userid Userid which you want to verify.
 * @param[in] template_data Data of a template.
 * @param[in] template_len Length of template data.
 * @return
 *      0 on success,
 *      -1 on failure.
 */
int bio_verify_by_template_data(int fd, unsigned long *userid,  unsigned char* template_data, int template_len) ;

/**@brief
 *      verify the finger template in a file against live scan.         
 * @param[in] fd Return of bio_init.
 * @param[in] template_file_name Template file name in which the template is available.
 * @param[in] sensor_timeout_period Timeout in seconds.
 * @return
 *      0 on success,
 *      -1 on failure.
 */
int bio_verify_host_template_file_by_scan(int fd, char *template_file_name, int sensor_timeout_period) ;

/**@brief
 *      Verify the finger template data against live scan.
 * @param[in] fd Return of bio_init.
 * @param[in] template_data Template data buffer.
 * @param[in] template_len size of template.
 * @param[in] sensor_timeout_period Timeout in seconds.
 * @return
 *      0 on success,
 *      -1 on failure.
 */
int bio_verify_host_template_data_by_scan(int fd,unsigned char *template_data, int template_len, int sensor_timeout_period) ;

/**@brief
 *      Verify a Finger by live scan against Multiple templates stored in the sensor module.
 * @param[in] fd Return of bio_init.
 * @param[in] template_data Template buffer (array of templates, max 10 ).
 * @param[in] template_len Templates lengths (array of template length)
 * @param[in] nTemplate Total number of Templates.
 * @param[in] sensor_timeout_period Timeout in seconds.
 * @return
 *      0 on success,
 *      -1 on failure.
 */
int  bio_verify_multiple_host_template_data_by_scan(int fd, unsigned char **template_data, int *template_len, int nTemplate, int sensor_timeout_period) ;

/**@brief
 *      Identify the finger by scanning against the enrolled templates in the module.
 * @param[in] fd Return of bio_init.
 * @param[out] userid On return contains corresponding userid in which the template is matched against.
 * @param[in] sensor_timeout_period Timeout in seconds.
 * @return
 *      0 on success,
 *      -1 on failure.
 */
int bio_identify_by_scan(int fd, unsigned long *userid, int sensor_timeout_period) ;

/**@brief
 *      Identify the finger template stored in the module against the template file.
 * @param[in] fd Return of bio_init.
 * @param[in] template_file_name Template file name in which the template is available.
 * @param[out] userid On return contains corresponding userid in which the template is  matched against.
 * @return
 *      0 on success,
 *      -1 on failure.
 */
int bio_identify_by_template_file(int fd, char *template_file_name, unsigned long *userid) ;

/**@brief
 *      Identify the finger template stored in the module against the template data
 * @param[in] fd Return of bio_init.
 * @param[out] userid On return contains corresponding userid in which the template is matched against
 * @param[in] template_data Template data.
 * @param[in] template_len Length of template data.
 * @return
 *      0 on success,
 *      -1 on failure.
 */
int bio_identify_by_template_data(int fd, unsigned long *userid,  unsigned char* template_data, int template_len) ;

/**@brief
 *      Delete the user from the module
 * @param[in] fd Return of bio_init.
 * @param[in] userid Userid to delete.
 * @return
 *      0 on success,
 *      -1 on failure.
 */
int bio_delete_template(int fd, unsigned long * userid);

/**@brief
 *      Delete multiple templates stored in the sensor module           
 * @param[in] fd Return of bio_init.
 * @param[in] start_id Starting userid.
 * @param[in] end_id And ending userid to delete.
 * @return
 *      0 on success,
 *      -1 on failure.
 */
int bio_delete_multiple_templates(int fd, unsigned long *start_id, unsigned long *end_id) ;

/**@brief
 *      Delete all templates in Module.         
 * @param[in] fd Return of bio_init.
 * @return
 *      0 on success,
 *      -1 on failure.
 */
int bio_delete_all_templates(int fd) ;

/**@brief
 *      Verify the raw fingerprint image against live scan
 * @param[in] fd Return of bio_init.
 * @param[in] image_data Raw image data.
 * @param[in] height Height of a raw image.
 * @param[in] width Width of a raw image.
 * @param[in] sensor_timeout_period Timeout in seconds.
 * @return
 *      0 on success,
 *      -1 on failure.
 * @note
 *      This API is not applicable for Optical scanners.
 */
int bio_verify_host_image_data_by_scan(int fd, unsigned char *image_data, int height, int width, int sensor_timeout_period) ;

/**@brief
 *      Convert the WSQFiles to RAWFiles & RAWFiles to WSQFiles 
 * @param[in] fd Return of bio_init.
 * @param[in] Con_Type RAW2WSQ or WSQ2RAW specifies type of conversion
 * @param[in] Src_File Source file contain required input data format.
 * @param[out] Dst_File On success file contain data RAW or WSQ as provide in Conv_Type.
 * @return
 *      0 on success,
 *      -1 on failure.
 */
int bio_Image_Conversion(int Conv_Type, char *Src_File, char *Dst_File);

/**@brief
 *      Converts the ISO to BIR & BIR to ISO files      
 * @param[in] Conv_Type ISO2BIR or BIR2ISO specifies type of conversion.
 * @param[in] lenInput Length of the input buffer
 * @param[in] pInputBuffer Input data buffer
 * @param[out] OutFileName On success file contain data either BIR or ISO as provide in Conv_Type.
 * @return
 *      0 on success,
 *      -1 on failure.
 */
int bio_Template_Conversion(int Conv_Type, unsigned int lenInput, unsigned char *pInputBuffer, char *OutFileName);

/**@brief
 *      Store the template into specified file by live scan with provided format.
 * @param[in] fd Return of bio_init.
 * @param[in] template_file_name Name of the template to be saved as.
 * @param[in]  sensor_timeout_period Timeout in seconds.
 * @param[out] quality Fingerprint image quality.
 * @param[in] TempFormat Format of the template.
 * @return
 *      0 on success,
 *      -1 on failure.
 */
int bio_scan_provided_template(int fd, char *template_file_name, int sensor_timeout_period, int quality,int TempFormat);

/**@brief
 *      Add the finger data to database with provided format.
 * @param[in] fd Return of bio_init.
 * @param[out] userid On return contain userid for a enrolled finger.
 * @param[in] template_data Template data.
 * @param[in] template_len Length of a template data.
 * @param[in] TempFormat Format of the template.
 * @return
 *      0 on success,
 *      -1 on failure.
 */
int bio_enroll_by_template_by_provided(int fd, unsigned long *userid, unsigned char *template_data, int template_len,int TempFormat);

/**@brief
 *      Identify the finger template stored in the module against the template file with provided format
 * @param[in] fd Return of bio_init.
 * @param[in] template_file_name Template file name in which the template is available.
 * @param[out] userid On return contains corresponding userid in which the template is  matched against.
 * @param[in] TempFormat Format of the template.
 * @return
 *      0 on success,
 *      -1 on failure.
 */
int bio_identify_by_template_file_by_format(int fd, char *template_file_name, unsigned long *userid,int TempFormat);




