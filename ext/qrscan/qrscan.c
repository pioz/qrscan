#include "ruby.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zbar.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Cleanup handler for the image data
void free_image(zbar_image_t *img) {
  const void *data = zbar_image_get_data(img);
  if (data) free((void *)data);
}

unsigned char* load_image(const char* image_path, int *width, int *height) {
  int channels;
  // Load image using stb_image
  unsigned char *img = stbi_load(image_path, width, height, &channels, 0);
  if (!img) {
    return NULL;
  }
  int pixels = (*width) * (*height);
  // Convert image to grayscale
  unsigned char *gray = malloc(pixels);
  if (!gray) {
    stbi_image_free(img);
    return NULL;
  }

  if (channels == 1) {
    memcpy(gray, img, pixels);
  } else {
    for (int i = 0; i < pixels; i++) {
      int r = img[i * channels];
      int g = img[i * channels + 1];
      int b = img[i * channels + 2];
      // Compute luminance using standard coefficients
      gray[i] = (unsigned char)(0.299 * r + 0.587 * g + 0.114 * b);
    }
  }

  stbi_image_free(img);
  return gray;
}

const char* scan_image(unsigned char* image, int width, int height) {
  const char *decoded_string = "";
  // Initialize zbar image scanner
  zbar_image_scanner_t *scanner = zbar_image_scanner_create();
  // Enable all barcode types
  zbar_image_scanner_set_config(scanner, 0, ZBAR_CFG_ENABLE, 1);
  // Wrap the grayscale image data in a zbar image
  zbar_image_t *zbar_img = zbar_image_create();
  // Set image format to "Y800" (8-bit grayscale)
  zbar_image_set_format(zbar_img, *(int*)"Y800");
  zbar_image_set_size(zbar_img, width, height);
  // Pass our cleanup handler instead of free
  zbar_image_set_data(zbar_img, image, width * height, free_image);
  // Scan the image for barcodes/QR codes
  zbar_scan_image(scanner, zbar_img);
  // Extract the results
  const zbar_symbol_t *symbol = zbar_image_first_symbol(zbar_img);
  if (symbol) {
    decoded_string = strdup(zbar_symbol_get_data(symbol));
  }

  // Cleanup
  zbar_image_destroy(zbar_img);
  zbar_image_scanner_destroy(scanner);

  return decoded_string;
}

VALUE qrscan_error;

VALUE scan(VALUE self, VALUE image_path) {
  const char *image_path_ptr = StringValuePtr(image_path);
  int width, height;
  unsigned char *img = load_image(image_path_ptr, &width, &height);
  if (!img) {
    rb_raise(qrscan_error, "Error loading image");
  }
  const char* s = scan_image(img, width, height);
  return rb_str_new2(s);
}

void Init_qrscan() {
  VALUE qrscan = rb_define_module("Qrscan");
  rb_define_singleton_method(qrscan, "scan", scan, 1);

  qrscan_error = rb_define_class_under(qrscan, "QrscanError", rb_eStandardError);
}
