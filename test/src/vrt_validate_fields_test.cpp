#include <gtest/gtest.h>

#include <vrt/vrt_common.h>
#include <vrt/vrt_validate.h>

class ValidateFieldsTest : public ::testing::Test {
   protected:
    ValidateFieldsTest() : h_(), f_() {}

    void SetUp() override {
        vrt_init_header(&h_);
        vrt_init_fields(&f_);
    }

    vrt_header h_;
    vrt_fields f_;
};

TEST_F(ValidateFieldsTest, None) {
    ASSERT_EQ(vrt_validate_fields(&h_, &f_), 0);
}

TEST_F(ValidateFieldsTest, OuiNoClassId) {
    h_.has.class_id = false;
    f_.class_id.oui = 0x01FFFFFF;
    ASSERT_EQ(vrt_validate_fields(&h_, &f_), 0);
}

TEST_F(ValidateFieldsTest, OuiValid) {
    h_.has.class_id = true;
    f_.class_id.oui = 0x00FFFFFF;
    ASSERT_EQ(vrt_validate_fields(&h_, &f_), 0);
}

TEST_F(ValidateFieldsTest, OuiInvalid) {
    h_.has.class_id = true;
    f_.class_id.oui = 0x01FFFFFF;
    ASSERT_EQ(vrt_validate_fields(&h_, &f_), VRT_ERR_OUI);
}
