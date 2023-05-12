#ifndef SOURCE_FORMATS_PRO_PARSE_FLAGS_H
#define SOURCE_FORMATS_PRO_PARSE_FLAGS_H

void yc_res_pro_parse_action_flags(unsigned char flags, yc_res_pro_action_flags_t *into);
void yc_res_pro_parse_weapon_flags(unsigned char flags, yc_res_pro_weapon_flags_t *into);

void yc_res_pro_parse_light_passage(unsigned char flags, yc_res_pro_light_passage_t *into);

#endif //SOURCE_FORMATS_PRO_PARSE_FLAGS_H
