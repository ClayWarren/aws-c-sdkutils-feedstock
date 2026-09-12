#include <aws/sdkutils/resource_name.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    const char *text = "arn:aws:s3:::example-bucket/raster.tif";
    struct aws_byte_cursor input = aws_byte_cursor_from_c_str(text);
    struct aws_resource_name arn;
    uint8_t storage[128];
    struct aws_byte_buf output = aws_byte_buf_from_empty_array(storage, sizeof(storage));
    size_t length = 0;
    aws_sdkutils_library_init(aws_default_allocator());
    int result = 1;
    if (aws_resource_name_init_from_cur(&arn, &input)) goto cleanup;
    if (arn.service.len != 2 || memcmp(arn.service.ptr, "s3", 2) || arn.region.len || arn.account_id.len) goto cleanup;
    if (aws_resource_name_length(&arn, &length) || length != strlen(text)) goto cleanup;
    if (aws_byte_buf_append_resource_name(&output, &arn) || output.len != length || memcmp(output.buffer, text, length)) goto cleanup;
    puts("Installed S3 resource-name parse/serialize round trip passed");
    result = 0;
cleanup:
    aws_sdkutils_library_clean_up();
    return result;
}
