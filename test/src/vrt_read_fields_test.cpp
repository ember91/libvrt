#include <gtest/gtest.h>

#include <any>
#include <array>
#include <cstdint>
#include <map>
#include <string>

#include <vrt/vrt_error_code.h>
#include <vrt/vrt_init.h>
#include <vrt/vrt_read.h>
#include <vrt/vrt_types.h>

#include "hex.h"
#include "init_garbage.h"
#include "read_util.h"

class ReadFieldsTest : public ::testing::Test {
   protected:
    ReadFieldsTest() : h_(), f_(), buf_() {}

    void SetUp() override {
        vrt_init_header(&h_);
        init_garbage_fields(&f_);
        buf_.fill(0xBAADF00D);
    }

    vrt_header              h_;
    vrt_fields              f_;
    std::array<uint32_t, 8> buf_;
};

/**
 * Assert fields section values.
 *
 * \param f      Fields.
 * \param values Lists non-default values, i.e. values that differ from the vrt_init_fields() state, as field name ->
 *               field value.
 */
static void assert_fields(const vrt_fields& f, const std::map<std::string, std::any>& values) {
    std::map<std::string, std::any> val_cp(values);
    ASSERT_EQ(Hex(f.stream_id), Hex(get_val<uint32_t>(&val_cp, "stream_id", 0)));
    ASSERT_EQ(Hex(f.class_id.oui), Hex(get_val<uint32_t>(&val_cp, "class_id.oui", 0)));
    ASSERT_EQ(Hex(f.class_id.information_class_code),
              Hex(get_val<uint16_t>(&val_cp, "class_id.information_class_code", 0)));
    ASSERT_EQ(Hex(f.class_id.packet_class_code), Hex(get_val<uint16_t>(&val_cp, "class_id.packet_class_code", 0)));
    ASSERT_EQ(Hex(f.integer_seconds_timestamp), Hex(get_val<uint32_t>(&val_cp, "integer_seconds_timestamp", 0)));
    ASSERT_EQ(Hex(f.fractional_seconds_timestamp), Hex(get_val<uint64_t>(&val_cp, "fractional_seconds_timestamp", 0)));

    check_remaining(val_cp);
}

TEST_F(ReadFieldsTest, NegativeSizeBuffer) {
    ASSERT_EQ(vrt_read_fields(&h_, buf_.data(), -1, &f_, true), VRT_ERR_BUFFER_SIZE);
}

TEST_F(ReadFieldsTest, None) {
    ASSERT_EQ(vrt_read_fields(&h_, buf_.data(), 0, &f_, true), 0);
}

TEST_F(ReadFieldsTest, StreamId1) {
    h_.packet_type = VRT_PT_IF_DATA_WITH_STREAM_ID;
    buf_[0]        = 0xFEFEBEBE;
    ASSERT_EQ(vrt_read_fields(&h_, buf_.data(), 1, &f_, true), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_fields(f_, {{"stream_id", static_cast<uint32_t>(0xFEFEBEBE)}});
}

TEST_F(ReadFieldsTest, StreamId2) {
    h_.packet_type = VRT_PT_EXT_DATA_WITHOUT_STREAM_ID;
    ASSERT_EQ(vrt_read_fields(&h_, buf_.data(), 0, &f_, true), 0);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_fields(f_, {});
}

TEST_F(ReadFieldsTest, StreamId3) {
    h_.packet_type = VRT_PT_EXT_DATA_WITH_STREAM_ID;
    buf_[0]        = 0xFEFEBEBE;
    ASSERT_EQ(vrt_read_fields(&h_, buf_.data(), 0, &f_, true), VRT_ERR_BUFFER_SIZE);
    ASSERT_EQ(vrt_read_fields(&h_, buf_.data(), 1, &f_, true), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_fields(f_, {{"stream_id", static_cast<uint32_t>(0xFEFEBEBE)}});
}

TEST_F(ReadFieldsTest, StreamId4) {
    h_.packet_type = VRT_PT_IF_CONTEXT;
    buf_[0]        = 0xFEFEBEBE;
    ASSERT_EQ(vrt_read_fields(&h_, buf_.data(), 0, &f_, true), VRT_ERR_BUFFER_SIZE);
    ASSERT_EQ(vrt_read_fields(&h_, buf_.data(), 1, &f_, true), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_fields(f_, {{"stream_id", static_cast<uint32_t>(0xFEFEBEBE)}});
}

TEST_F(ReadFieldsTest, StreamId5) {
    h_.packet_type = VRT_PT_EXT_CONTEXT;
    buf_[0]        = 0xFEFEBEBE;
    ASSERT_EQ(vrt_read_fields(&h_, buf_.data(), 0, &f_, true), VRT_ERR_BUFFER_SIZE);
    ASSERT_EQ(vrt_read_fields(&h_, buf_.data(), 1, &f_, true), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_fields(f_, {{"stream_id", static_cast<uint32_t>(0xFEFEBEBE)}});
}

TEST_F(ReadFieldsTest, ClassIdOui) {
    h_.has.class_id = true;
    buf_[0]         = 0x00FEDCBA;
    buf_[1]         = 0x00000000;
    ASSERT_EQ(vrt_read_fields(&h_, buf_.data(), 2, &f_, true), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_fields(f_, {{"class_id.oui", static_cast<uint32_t>(0x00FEDCBA)}});
}

TEST_F(ReadFieldsTest, ClassIdInformationClassCode) {
    h_.has.class_id = true;
    buf_[0]         = 0x00000000;
    buf_[1]         = 0xABAB0000;
    ASSERT_EQ(vrt_read_fields(&h_, buf_.data(), 2, &f_, true), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_fields(f_, {{"class_id.information_class_code", static_cast<uint16_t>(0xABAB)}});
}

TEST_F(ReadFieldsTest, ClassIdPacketClassCode) {
    h_.has.class_id = true;
    buf_[0]         = 0x00000000;
    buf_[1]         = 0x0000CBCB;
    ASSERT_EQ(vrt_read_fields(&h_, buf_.data(), 2, &f_, true), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_fields(f_, {{"class_id.packet_class_code", static_cast<uint16_t>(0xCBCB)}});
}

TEST_F(ReadFieldsTest, ClassIdReserved) {
    h_.has.class_id = true;
    buf_[0]         = 0xFF000000;
    buf_[1]         = 0x00000000;
    ASSERT_EQ(vrt_read_fields(&h_, buf_.data(), 2, &f_, true), VRT_ERR_RESERVED);
    ASSERT_EQ(vrt_read_fields(&h_, buf_.data(), 2, &f_, false), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_fields(f_, {});
}

TEST_F(ReadFieldsTest, IntegerSecondsTimestamp) {
    h_.tsi  = VRT_TSI_OTHER;
    buf_[0] = 0x12345678;
    ASSERT_EQ(vrt_read_fields(&h_, buf_.data(), 0, &f_, true), VRT_ERR_BUFFER_SIZE);
    ASSERT_EQ(vrt_read_fields(&h_, buf_.data(), 1, &f_, true), 1);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_fields(f_, {{"integer_seconds_timestamp", static_cast<uint32_t>(0x12345678)}});
}

TEST_F(ReadFieldsTest, FractionalSecondsTimestamp) {
    h_.tsf  = VRT_TSF_REAL_TIME;
    buf_[0] = 0x000000E8;
    buf_[1] = 0xD4A50FFF;
    ASSERT_EQ(vrt_read_fields(&h_, buf_.data(), 1, &f_, true), VRT_ERR_BUFFER_SIZE);
    ASSERT_EQ(vrt_read_fields(&h_, buf_.data(), 2, &f_, true), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_fields(f_, {{"fractional_seconds_timestamp", static_cast<uint64_t>(0x000000E8D4A50FFF)}});
}

TEST_F(ReadFieldsTest, FractionalSecondsTimestampInvalid) {
    h_.tsf  = VRT_TSF_REAL_TIME;
    buf_[0] = 0x000000E8;
    buf_[1] = 0xD4A51000;
    ASSERT_EQ(vrt_read_fields(&h_, buf_.data(), 2, &f_, true), VRT_ERR_BOUNDS_REAL_TIME);
    ASSERT_EQ(vrt_read_fields(&h_, buf_.data(), 2, &f_, false), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_fields(f_, {{"fractional_seconds_timestamp", static_cast<uint64_t>(0x000000E8D4A51000)}});
}

TEST_F(ReadFieldsTest, EveryOther1) {
    h_.packet_type = VRT_PT_IF_DATA_WITH_STREAM_ID;
    h_.tsi         = VRT_TSI_UTC;
    buf_[0]        = 0xABABCBCB;
    buf_[1]        = 0xCECEBEBE;
    ASSERT_EQ(vrt_read_fields(&h_, buf_.data(), 1, &f_, true), VRT_ERR_BUFFER_SIZE);
    ASSERT_EQ(vrt_read_fields(&h_, buf_.data(), 2, &f_, true), 2);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_fields(f_, {{"stream_id", static_cast<uint32_t>(0xABABCBCB)},
                       {"integer_seconds_timestamp", static_cast<uint32_t>(0xCECEBEBE)}});
}

TEST_F(ReadFieldsTest, EveryOther2) {
    h_.has.class_id = true;
    h_.tsf          = VRT_TSF_FREE_RUNNING_COUNT;
    buf_[0]         = 0x00FEDCBA;
    buf_[1]         = 0xABABCBCB;
    buf_[2]         = 0xCECEBEBE;
    buf_[3]         = 0xBABBA011;
    ASSERT_EQ(vrt_read_fields(&h_, buf_.data(), 3, &f_, true), VRT_ERR_BUFFER_SIZE);
    ASSERT_EQ(vrt_read_fields(&h_, buf_.data(), 4, &f_, true), 4);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_fields(f_, {{"class_id.oui", static_cast<uint32_t>(0x00FEDCBA)},
                       {"class_id.information_class_code", static_cast<uint16_t>(0xABAB)},
                       {"class_id.packet_class_code", static_cast<uint16_t>(0xCBCB)},
                       {"fractional_seconds_timestamp", static_cast<uint64_t>(0xCECEBEBEBABBA011)}});
}

TEST_F(ReadFieldsTest, All) {
    h_.packet_type  = VRT_PT_IF_DATA_WITH_STREAM_ID;
    h_.has.class_id = true;
    h_.tsi          = VRT_TSI_GPS;
    h_.tsf          = VRT_TSF_FREE_RUNNING_COUNT;
    buf_[0]         = 0xCECEBEBE;
    buf_[1]         = 0x00FEDCBA;
    buf_[2]         = 0xABABBCBC;
    buf_[3]         = 0xCECEBEBE;
    buf_[4]         = 0xADDABEBB;
    buf_[5]         = 0xEFEFEF01;
    ASSERT_EQ(vrt_read_fields(&h_, buf_.data(), 5, &f_, true), VRT_ERR_BUFFER_SIZE);
    ASSERT_EQ(vrt_read_fields(&h_, buf_.data(), 6, &f_, true), 6);
    SCOPED_TRACE(::testing::UnitTest::GetInstance()->current_test_info()->name());
    assert_fields(f_, {{"stream_id", static_cast<uint32_t>(0xCECEBEBE)},
                       {"class_id.oui", static_cast<uint32_t>(0x00FEDCBA)},
                       {"class_id.information_class_code", static_cast<uint16_t>(0xABAB)},
                       {"class_id.packet_class_code", static_cast<uint16_t>(0xBCBC)},
                       {"integer_seconds_timestamp", static_cast<uint32_t>(0xCECEBEBE)},
                       {"fractional_seconds_timestamp", static_cast<uint64_t>(0xADDABEBBEFEFEF01)}});
}
