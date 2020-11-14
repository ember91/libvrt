#ifndef INCLUDE_VRT_VRT_TYPES_H_
#define INCLUDE_VRT_VRT_TYPES_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Error return code.
 */
static const int32_t VRT_ERR = -1;

/**
 * Type of packet.
 *
 * \note Remaining values are reserved for future packet types.
 */
typedef enum vrt_packet_type {
    VRT_PT_IF_DATA_WITHOUT_STREAM_ID  = 0x0, /**< IF data packet without stream identifier. */
    VRT_PT_IF_DATA_WITH_STREAM_ID     = 0x1, /**< IF data packet with stream identifier. */
    VRT_PT_EXT_DATA_WITHOUT_STREAM_ID = 0x2, /**< Extension data packet without stream identifier. */
    VRT_PT_EXT_DATA_WITH_STREAM_ID    = 0x3, /**< Extension data packet with stream identifier. */
    VRT_PT_IF_CONTEXT                 = 0x4, /**< IF context packet. */
    VRT_PT_EXT_CONTEXT                = 0x5  /**< Extension context packet. */
} vrt_packet_type;

/**
 * Indicates presence of header fields.
 */
typedef struct vrt_header_indicators {
    /** True if there is class identifier information. */
    bool class_id;
    /**
     *  True if there is a trailer word.
     *
     * \note True is only valid for IF and Extension data packets.
     */
    bool trailer;
} vrt_header_indicators;

/**
 * Reference-point time in resolution of a second.
 *
 * \note Rule 6.1.1-7: All the packets in an IF Data Packet Stream shall have the same TSI code, and thus the same
 *       Integer-seconds Timestamp.
 */
typedef enum vrt_tsi {
    VRT_TSI_NONE      = 0x0, /**< No one-second resolution time. */
    VRT_TSI_UNDEFINED = 0x0, /**< Replaces NONE in the Formatted GPS/INS geolocation fields. */
    VRT_TSI_UTC       = 0x1, /**< Seconds since January 1, 1970, GMT. */
    VRT_TSI_GPS       = 0x2, /**< Seconds since January 6, 1980. */
    VRT_TSI_OTHER     = 0x3  /**< Some other time reference. */
} vrt_tsi;

/**
 * Reference-point time in subsecond resolution.
 *
 * \note Rule 6.1.1-10: All the packets in an IF Data Packet Stream shall have the same TSF code, and thus the same
 *       Fractional-seconds Timestamp.
 */
typedef enum vrt_tsf {
    VRT_TSF_NONE               = 0x0, /**< No subsecond time resolution. */
    VRT_TSF_UNDEFINED          = 0x0, /**< Replaces NONE in the Formatted GPS/INS geolocation fields. */
    VRT_TSF_SAMPLE_COUNT       = 0x1, /**< Number of samples since last second shift. */
    VRT_TSF_REAL_TIME          = 0x2, /**< Picoseconds since last second shift. */
    VRT_TSF_FREE_RUNNING_COUNT = 0x3  /**< Counts samples from the first sample. Will eventually roll over. */
} vrt_tsf;

/**
 * Header data.
 */
typedef struct vrt_header {
    /** Type of packet. */
    vrt_packet_type packet_type;
    /** Indicates presence of extra fields. */
    vrt_header_indicators has;
    /**
     * Timestamp mode. Whether timestamps are with fine (false) or coarse (true) resolution. Coarse if an event
     * happened sometime in the data sampling interval.
     *
     * \note True is only valid for context packets.
     */
    bool tsm;
    /**
     * Type of integer second timestamp.
     *
     * \note Rule 6.1.1-7: All the packets in an IF Data Packet Stream shall have the same TSI code, and thus the same
     *       Integer-seconds Timestamp.
     */
    vrt_tsi tsi;
    /**
     * Type of fractional second timestamp.
     *
     * \note Rule 6.1.1-10: All the packets in an IF Data Packet Stream shall have the same TSF code, and thus the same
     *       Fractional-seconds Timestamp.
     */
    vrt_tsf tsf;
    /**
     * Modulo-16 count of packets for the specified stream. Can (to some degree) be used to determine packet loss.
     *
     * \note Only the 4 least significant bits are used.
     * \note Rule 6.1.1-11: The Packet Count shall increment in each consecutive IF Data packet having the same Stream
     *       Identifier and the same packet type. The count shall roll over from “1111” to “0000”.
     */
    uint8_t packet_count;
    /**
     * Total number of 32-bit words in the packet.
     *
     * \note May vary from packet to packet.
     */
    uint16_t packet_size;
} vrt_header;

/**
 * Class identifier. Determines data origin.
 */
typedef struct vrt_class_identifier {
    /**
     * IEEE Organizationally Unique Identifier. Identifies a company.
     *
     * \note Only the 24 least significant bits are used.
     */
    uint32_t oui;
    /** Information class to which this packet stream belongs. */
    uint16_t information_class_code;
    /** Packet class of company to which this packet stream belongs. */
    uint16_t packet_class_code;
} vrt_class_identifier;

/**
 * Field data.
 */
typedef struct vrt_fields {
    /** Identifies which stream it belongs to. Activate by setting packet_type accordingly.
     *
     * \note Although this field is optional for IF data packets it is required for context packets.
     */
    uint32_t stream_id;
    /** Determines data origin. Activate by setting has.class_id. */
    vrt_class_identifier class_id;
    /**
     * Integer second timestamp as specified by tsi. Activate by setting tsi to anything but
     * VRT_TSI_NONE/VRT_TSI_UNDEFINED.
     */
    uint32_t integer_seconds_timestamp;
    /** Fractional second timestamp as specified by tsf. Activate by setting tsf to anything but
     * VRT_TSF_NONE/VRT_TSF_UNDEFINED.
     */
    uint64_t fractional_seconds_timestamp;
} vrt_fields;

/**
 * Indicates presence of trailer fields.
 */
typedef struct vrt_trailer_indicators {
    bool calibrated_time;                 /**< True if it has the Calibrated time indicator field. */
    bool valid_data;                      /**< True if it has the Valid data indicator field. */
    bool reference_lock;                  /**< True if it has the Reference lock indicator field. */
    bool agc_or_mgc;                      /**< True if it has the AGC/MGC indicator field. */
    bool detected_signal;                 /**< True if it has the Detected signal indicator field. */
    bool spectral_inversion;              /**< True if it has the Spectral inversion indicator field. */
    bool over_range;                      /**< True if it has the Over-range indicator field. */
    bool sample_loss;                     /**< True if it has the Sample loss indicator field. */
    bool user_defined11;                  /**< True if it has the User defined bit 11 indicator field. */
    bool user_defined10;                  /**< True if it has the User defined bit 10 indicator field. */
    bool user_defined9;                   /**< True if it has the User defined bit 9 indicator field. */
    bool user_defined8;                   /**< True if it has the User defined bit 8 indicator field. */
    bool associated_context_packet_count; /**< True if it has the Associated context packet count field. */
} vrt_trailer_indicators;

/**
 * Trailer section data.
 */
typedef struct vrt_trailer {
    /** Field presence indicators. */
    vrt_trailer_indicators has;
    /**
     * True if the timestamp is calibrated to an external reference. Activate by setting has.calibrated_time to true.
     */
    bool calibrated_time;
    /** True if the packet data is valid. Activate by setting has.valid_data to true. */
    bool valid_data;
    /** True if the reference is phase locked and stable. Activate by setting has.reference_lock to true. */
    bool reference_lock;
    /** True if AGC is active, False if MGC is. Activate by setting has.agc_or_mgc to true. */
    bool agc_or_mgc;
    /** True if a signal is detected in the packet. Activate by setting has.detected_signal to true. */
    bool detected_signal;
    /** True if the signal has an inverted spectrum. Activate by setting has.spectral_inversion to true. */
    bool spectral_inversion;
    /**
     * True if at least one data sample in the packet exceeds the range of a sample. Activate by setting has.over_range
     * to true.
     */
    bool over_range;
    /** Contains at least one sample discontinuity. Activate by setting has.sample_loss to true. */
    bool sample_loss;
    /** Bit 11 in the trailer word. Defined by the user. Activate by setting has.user_defined11 to true. */
    bool user_defined11;
    /** Bit 10 in the trailer word. Defined by the user. Activate by setting has.user_defined10 to true. */
    bool user_defined10;
    /** Bit 9 in the trailer word. Defined by the user. Activate by setting has.user_defined9 to true. */
    bool user_defined9;
    /** Bit 8 in the trailer word. Defined by the user. Activate by setting has.user_defined8 to true. */
    bool user_defined8;
    /**
     * Number of context packets associated to this packet. Activate by setting has.associated_context_packet_count to
     * True.
     *
     * \note Only the 7 least significant bits are used.
     */
    uint8_t associated_context_packet_count;
} vrt_trailer;

/**
 * Indicates presence of context fields.
 */
typedef struct vrt_context_indicators {
    bool reference_point_identifier;     /**< True if it has the Reference point identifier field. */
    bool bandwidth;                      /**< True if it has the Bandwidth field. */
    bool if_reference_frequency;         /**< True if it has the IF reference frequency field. */
    bool rf_reference_frequency;         /**< True if it has the RF reference frequency field. */
    bool rf_reference_frequency_offset;  /**< True if it has the RF reference frequency offset field. */
    bool if_band_offset;                 /**< True if it has the IF band offset field. */
    bool reference_level;                /**< True if it has the Reference level field. */
    bool gain;                           /**< True if it has the Gain stage 1 and 2 fields.*/
    bool over_range_count;               /**< True if it has the Over-range count field. */
    bool sample_rate;                    /**< True if it has the Sample rate field. */
    bool timestamp_adjustment;           /**< True if it has the Timestamp adjustment field. */
    bool timestamp_calibration_time;     /**< True if it has the Timestamp calibration time field. */
    bool temperature;                    /**< True if it has the Temperature field. */
    bool device_identifier;              /**< True if it has the Device identifier field. */
    bool state_and_event_indicators;     /**< True if it has the State and event indicators field. */
    bool data_packet_payload_format;     /**< True if it has the Data packet payload format field. */
    bool formatted_gps_geolocation;      /**< True if it has the Formatted GPS geolocation field. */
    bool formatted_ins_geolocation;      /**< True if it has the Formatted INS geolocation field. */
    bool ecef_ephemeris;                 /**< True if it has the ECEF ephemeris field. */
    bool relative_ephemeris;             /**< True if it has the Relative ephemeris field. */
    bool ephemeris_reference_identifier; /**< True if it has the Ephemeris reference identifier field. */
    bool gps_ascii;                      /**< True if it has the GPS ASCII field. */
    bool context_association_lists;      /**< True if it has the Context association list field. */
} vrt_context_indicators;

/**
 * Signal gain or attenuation.
 */
typedef struct vrt_gain {
    float stage1; /**< Front-end or RF gain [dB]. */
    float stage2; /**< Back-end or IF gain [dB]. */
} vrt_gain;

/**
 * Identifies manufacturer and model of device generating context stream.
 */
typedef struct vrt_device_identifier {
    /**
     * IEEE Organizationally Unique Identifier (company identifier) of device.
     *
     * \note Only the 24 least significant bits are used.
     */
    uint32_t oui;
    /** Model identification. */
    uint16_t device_code;
} vrt_device_identifier;

/**
 * Indicates presence of vrt_state_and_event fields.
 */
typedef struct vrt_state_and_event_indicators {
    bool calibrated_time;    /**< True if it has the Calibrated time indicator field. */
    bool valid_data;         /**< True if it has the Valid data indicator field. */
    bool reference_lock;     /**< True if it has the Reference lock indicator field. */
    bool agc_or_mgc;         /**< True if it has the AGC/MGC indicator field. */
    bool detected_signal;    /**< True if it has the Detected signal indicator field. */
    bool spectral_inversion; /**< True if it has the Spectral inversion indicator field. */
    bool over_range;         /**< True if it has the Over-range indicator field. */
    bool sample_loss;        /**< True if it has the Sample loss indicator field. */

} vrt_state_and_event_indicators;

/**
 * Binary state and event indicators.
 */
typedef struct vrt_state_and_event {
    /** Field presence indicators. */
    vrt_state_and_event_indicators has;
    /**
     * True if the timestamp is calibrated to an external reference. Activate by setting has.calibrated_time to true.
     */
    bool calibrated_time;
    /** True if the packet data is valid. Activate by setting has.valid_data to true. */
    bool valid_data;
    /** True if the reference is phase locked and stable. Activate by setting has.reference_lock to true. */
    bool reference_lock;
    /** True if AGC is active, False if MGC is active. Activate by setting has.agc_or_mgc to true. */
    bool agc_or_mgc;
    /** True if a signal is detected in the packet. Activate by setting has.detected_signal to true. */
    bool detected_signal;
    /** True if a signal in the data has an inverted spectrum. Activate by setting has.spectral_inversion to true. */
    bool spectral_inversion;
    /**
     * True if at least one data sample in the packet exceeds the range of a sample. Activate by setting has.over_range
     * to true.
     *
     * \note This field does not have to be persistent between context packets.
     */
    bool over_range;
    /**
     * Contains at least one sample discontinuity. Activate by setting has.sample_loss to true.
     *
     * \note This field does not have to be persistent between context packets.
     */
    bool sample_loss;
    /**
     * State and event bit 7. Defined by the user.
     *
     * \note This field does not have to be persistent between context packets.
     */
    bool user_defined7;
    /**
     * State and event bit 6. Defined by the user.
     *
     * \note This field does not have to be persistent between context packets.
     */
    bool user_defined6;
    /**
     * State and event bit 5. Defined by the user.
     *
     * \note This field does not have to be persistent between context packets.
     */
    bool user_defined5;
    /**
     * State and event bit 4. Defined by the user.
     *
     * \note This field does not have to be persistent between context packets.
     */
    bool user_defined4;
    /**
     * State and event bit 3. Defined by the user.
     *
     * \note This field does not have to be persistent between context packets.
     */
    bool user_defined3;
    /**
     * State and event bit 2. Defined by the user.
     *
     * \note This field does not have to be persistent between context packets.
     */
    bool user_defined2;
    /**
     * State and event bit 1. Defined by the user.
     *
     * \note This field does not have to be persistent between context packets.
     */
    bool user_defined1;
    /**
     * State and event bit 0. Defined by the user.
     *
     * \note This field does not have to be persistent between context packets.
     */
    bool user_defined0;
} vrt_state_and_event;

/**
 * Indicates data sample type.
 */
typedef enum vrt_real_complex {
    VRT_ROC_REAL              = 0x0, /**< Real number. */
    VRT_ROC_COMPLEX_CARTESIAN = 0x1, /**< Cartesian complex number. */
    VRT_ROC_COMPLEX_POLAR     = 0x2  /**< Polar complex number. */
} vrt_real_complex;

/**
 * Sample format type.
 */
typedef enum vrt_data_item_format {
    VRT_DIF_SIGNED_FIXED_POINT                       = 0x00,
    VRT_DIF_SIGNED_VRT_1_BIT_EXPONENT                = 0x01,
    VRT_DIF_SIGNED_VRT_2_BIT_EXPONENT                = 0x02,
    VRT_DIF_SIGNED_VRT_3_BIT_EXPONENT                = 0x03,
    VRT_DIF_SIGNED_VRT_4_BIT_EXPONENT                = 0x04,
    VRT_DIF_SIGNED_VRT_5_BIT_EXPONENT                = 0x05,
    VRT_DIF_SIGNED_VRT_6_BIT_EXPONENT                = 0x06,
    VRT_DIF_IEEE_754_SINGLE_PRECISION_FLOATING_POINT = 0x0E,
    VRT_DIF_IEEE_754_DOUBLE_PRECISION_FLOATING_POINT = 0x0F,
    VRT_DIF_UNSIGNED_FIXED_POINT                     = 0x10,
    VRT_DIF_UNSIGNED_VRT_1_BIT_EXPONENT              = 0x11,
    VRT_DIF_UNSIGNED_VRT_2_BIT_EXPONENT              = 0x12,
    VRT_DIF_UNSIGNED_VRT_3_BIT_EXPONENT              = 0x13,
    VRT_DIF_UNSIGNED_VRT_4_BIT_EXPONENT              = 0x14,
    VRT_DIF_UNSIGNED_VRT_5_BIT_EXPONENT              = 0x15,
    VRT_DIF_UNSIGNED_VRT_6_BIT_EXPONENT              = 0x16
} vrt_data_item_format;

/**
 * Format of data packet payload.
 *
 * \note Some fields regarding size contain a number that is one less than the actual size, while some contain a number
 *       equal to the actual size.
 */
typedef struct vrt_data_packet_payload_format {
    /** True if link-efficient packing is used. False if processing-efficient packing is used. */
    bool packing_method;
    /** Data sample type. */
    vrt_real_complex real_or_complex;
    /** Data sample format. */
    vrt_data_item_format data_item_format;
    /** True when sample component repeating is used. */
    bool sample_component_repeat;
    /**
     * Equal to event tag size in paired packet data stream.
     *
     * \note Only the 3 least significant bits are used.
     */
    uint8_t event_tag_size;
    /**
     * Equal to channel tag size in paired channel data stream.
     *
     * \note Only the 4 least significant bits are used.
     */
    uint8_t channel_tag_size;
    /**
     * One less than item packing fields size in paired channel data stream.
     *
     * \note Only the 6 least significant bits are used.
     */
    uint8_t item_packing_field_size;
    /**
     * One less than data item size in paired channel data stream.
     *
     * \note Only the 3 least significant bits are used.
     */
    uint8_t data_item_size;
    /** One less than repeat count in paired channel data stream. */
    uint16_t repeat_count;
    /** One less than vector size in paired channel data stream. */
    uint16_t vector_size;
} vrt_data_packet_payload_format;

/**
 * Geolocation data.
 */
typedef struct vrt_formatted_geolocation {
    /** Type of integer second timestamp. */
    vrt_tsi tsi;
    /** Type of fractional second timestamp. */
    vrt_tsf tsf;
    /**
     * IEEE Organizationally Unique Identifier of GPS/INS manufacturer.
     *
     * \note Only the 24 least significant bits are used.
     */
    uint32_t oui;
    /** Integer second timestamp of position fix as specified by tsi. */
    uint32_t integer_second_timestamp;
    /** Fractional second timestamp of position fix as specified by tsf. */
    uint64_t fractional_second_timestamp;
    /** Latitude [degrees]. */
    double latitude;
    /** Longitude [degrees]. */
    double longitude;
    /** Altitude [m]. */
    double altitude;
    /** Speed over ground [m/s]. */
    double speed_over_ground;
    /** Orientation with respect to true north [degrees]. */
    double heading_angle;
    /** Travel direction with respect to true north [degrees]. */
    double track_angle;
    /** Magnetic variation with respect to true north [degrees]. */
    double magnetic_variation;
} vrt_formatted_geolocation;

/**
 * Location in a specific coordinate system.
 */
typedef struct vrt_ephemeris {
    /** Type of integer second timestamp. */
    vrt_tsi tsi;
    /** Type of fractional second timestamp. */
    vrt_tsf tsf;
    /**
     * IEEE Organizationally Unique Identifier of GPS/INS manufacturer.
     *
     * \note Only the 24 least significant bits are valid.
     */
    uint32_t oui;
    /** Integer second timestamp of position fix as specified by tsi. */
    uint32_t integer_second_timestamp;
    /** Fractional second timestamp of position fix  as specified by tsf. */
    uint64_t fractional_second_timestamp;
    /**
     * x-position [m]. Along earth's equator. Positive direction is intersection of equator plane and prime meridian.
     */
    double position_x;
    /** y-position [m]. Completes a right-handed orthogonal system 90 degrees east of the x-axis. */
    double position_y;
    /** z-position [m]. Directed along earth's rotational axis, where north is positive. */
    double position_z;
    /** Altitude about z-axis [degrees]. Positive rotation is x to y. */
    double altitude_alpha;
    /** Altitude about y-axis [degrees]. Positive direction is x to z. */
    double altitude_beta;
    /** Altitude about x-axis [degrees]. Positive direction is y to z. */
    double altitude_phi;
    /** x-velocity [m/s]. Defined similarly to x-position. */
    double velocity_dx;
    /** y-velocity [m/s]. Defined similarly to y-position. */
    double velocity_dy;
    /** z-position [m/s]. Defined similarly to z-position. */
    double velocity_dz;
} vrt_ephemeris;

/**
 * GPS device ASCII.
 */
typedef struct vrt_gps_ascii {
    /**
     * IEEE Organizationally Unique Identifier of GPS/INS manufacturer.
     *
     * \note Only the 24 least significant bits are used.
     */
    uint32_t oui;
    /** Number of 32-bit words used for the GPS ASCII. */
    uint32_t number_of_words;
    /**
     * GPS ASCII sentences. Padded with null characters so it's a multiple of 4 B. Pointer may be null when
     * not defined.
     *
     * \warning Will point into the buffer when the field is read. Deallocating the buffer will render the pointer
     *          invalid.
     */
    const char* ascii;
} vrt_gps_ascii;

/**
 * Indicator fields for Context.
 */
typedef struct vrt_context_association_lists_indicators {
    /** True if asynchronous_channel_tag_list is included. */
    bool asynchronous_channel_tag_list;
} vrt_context_association_lists_indicators;

/**
 * Contains association lists.
 */
typedef struct vrt_context_association_lists {
    /**
     * Number of Stream IDs in source context association list.
     *
     * \note Only the 9 least significant bits are used.
     */
    uint16_t source_list_size;
    /**
     * Number of Stream IDs in system context association list.
     *
     * \note Only the 9 least significant bits are used.
     */
    uint16_t system_list_size;
    /** Number of Stream IDs in vector-component association list. */
    uint16_t vector_component_list_size;
    /** Field presence indicators. */
    vrt_context_association_lists_indicators has;
    /**
     * Number of Stream IDs in asynchronous-channel context association list.
     *
     * \note Only the 15 least significant bits are used.
     * \note Must have as many entries as sample vector dimension.
     */
    uint16_t asynchronous_channel_list_size;
    /**
     * Producer metadata.
     *
     * \warning Will point into the buffer when the field is read. Deallocating the buffer will render the pointer
     *          invalid.
     */
    const uint32_t* source_context_association_list;
    /**
     * Additional metadata not covered by other context association lists.
     *
     * \warning Will point into the buffer when the field is read. Deallocating the buffer will render the pointer
     *          invalid.
     */
    const uint32_t* system_context_association_list;
    /**
     * Holds metadata for data packet stream sample vectors.
     *
     * \warning Will point into the buffer when the field is read. Deallocating the buffer will render the pointer
     *          invalid.
     */
    const uint32_t* vector_component_context_association_list;
    /**
     * Associates a stream with each channel when channel tags are used in a data packet stream.
     *
     * \warning Will point into the buffer when the field is read. Deallocating the buffer will render the pointer
     *          invalid.
     */
    const uint32_t* asynchronous_channel_context_association_list;
    /**
     * All channel tags used in paired data packet stream.
     *
     * \warning Will point into the buffer when the field is read. Deallocating the buffer will render the pointer
     *          invalid.
     */
    const uint32_t* asynchronous_channel_tag_list;

} vrt_context_association_lists;

/**
 * Context section data.
 */
typedef struct vrt_if_context {
    /**
     * True if this packet contains new context information.
     *
     * \note This is sometimes called named indicator and sometimes identifier.
     */
    bool context_field_change_indicator;
    /** Field presence indicators. */
    vrt_context_indicators has;
    /** Stream ID of for which the context packet applies. */
    uint32_t reference_point_identifier;
    /**
     * Bandwidth [Hz].
     *
     * \note Negative values are not valid.
     * \warning This may lead to loss of precision, since the underlying VRT fixed point format has 64 bits while a
     *          double only has 53 bits of precision.
     */
    double bandwidth;
    /**
     * Center frequency [Hz].
     *
     * \warning This may lead to loss of precision, since the underlying VRT fixed point format has 64 bits while a
     *          double only has 53 bits of precision.
     */
    double if_reference_frequency;
    /**
     * Original center frequency before translation [Hz].
     *
     * \warning This may lead to loss of precision, since the underlying VRT fixed point format has 64 bits while a
     *          double only has 53 bits of precision.
     */
    double rf_reference_frequency;
    /**
     * Original frequency is the sum of this and rf_reference_frequency [Hz].
     *
     * \warning This may lead to loss of precision, since the underlying VRT fixed point format has 64 bits while a
     *          double only has 53 bits of precision.
     */
    double rf_reference_frequency_offset;
    /**
     * Offset from IF reference frequency to center of the band [Hz]. Band center is the sum of this and
     * if_reference_frequency.
     *
     * \warning This may lead to loss of precision, since the underlying VRT fixed point format has 64 bits while a
     *          double only has 53 bits of precision.
     */
    double if_band_offset;
    /** Physical signal amplitude at the Reference Point [dBm]. */
    float reference_level;
    /** Gain. */
    vrt_gain gain;
    /**
     * Number of over-range samples, i.e. samples with too high amplitude.
     *
     * \note This field does not have to be persistent between context packets.
     */
    uint32_t over_range_count;
    /**
     * Sample rate [Hz].
     *
     * \warning This may lead to loss of precision, since the underlying VRT fixed point format has 64 bits while a
     * double only has 53 bits of precision.
     */
    double sample_rate;
    /** System signal processing time [ps]. Offset from reference point time. */
    uint64_t timestamp_adjustment;
    /**
     * Time when timestamp was known to be correct, i.e. calibrated [s]. Represented in the same way as the TSI field.
     */
    uint32_t timestamp_calibration_time;
    /** Temperature of some component in the signal chain that may affect the signal. */
    float temperature;
    /** Identifies device carrying a context packet stream. */
    vrt_device_identifier device_identifier;
    /** Conveys state information. */
    vrt_state_and_event state_and_event_indicators;
    /** Describes formatting of data stream in data packets. */
    vrt_data_packet_payload_format data_packet_payload_format;
    /** GPS information. */
    vrt_formatted_geolocation formatted_gps_geolocation;
    /** INS information. */
    vrt_formatted_geolocation formatted_ins_geolocation;
    /** ECEF (earth-centered, earth-fixed) coordinates. */
    vrt_ephemeris ecef_ephemeris;
    /** Coordinates with user defined point of reference. */
    vrt_ephemeris relative_ephemeris;
    /** Stream ID of relative ephemeris origin. */
    uint32_t ephemeris_reference_identifier;
    /** GPS ASCII positional information. */
    vrt_gps_ascii gps_ascii;
    /**
     * Describes which data packets are associated to this context packet.
     *
     * \note This is sometimes called lists and sometimes list.
     */
    vrt_context_association_lists context_association_lists;
} vrt_if_context;

#ifdef __cplusplus
}
#endif

#endif
