/*

Dotify an array.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "private/dotify.h"



// Analyse an array from a pointer.
void _ctydot_ptrarray(_Cty_Die *die, void *data, size_t size)
{
    Dwarf_Attribute attr;
    Dwarf_Word word;

    dwarf_attr(&die->type, DW_AT_byte_size, &attr);
    dwarf_formudata(&attr, &word);

    Dwarf_Off off = dwarf_dieoffset(&die->qualifiers);


    // Dotify each element of the array.
    char *end = (char *)data + size;
    char *pos = data;

    for (; pos < end; pos += word)
        _ctydot_dotify_inline(off, pos);
}



// Dotify the content from an array.
void _ctydot_array_inline(_Cty_Die *fdie, void *data)
{
   _ctydot_array(fdie, data);
}



// Dotify an array.
void _ctydot_array(_Cty_Die *die, void *data)
{
    Dwarf_Attribute attr;
    Dwarf_Word word;
    Dwarf_Die inner_die;

    Dwarf_Die child; // subrange
    if (dwarf_child(&die->type, &child))
        return;

    dwarf_attr(&child, DW_AT_upper_bound, &attr);
    dwarf_formudata(&attr, &word);

    size_t items = word + 1; 

    dwarf_attr(&die->type, DW_AT_type, &attr);
    dwarf_formref_die(&attr, &inner_die);

    _Cty_Die full_inner;
    Dwarf_Off off = dwarf_dieoffset(&inner_die);
    _ctycat_format_die(off, &full_inner);

    dwarf_attr(&full_inner.type, DW_AT_byte_size, &attr);
    dwarf_formudata(&attr, &word);

    size_t item_size = word;

    // Analyse each element of the array.

    _ctydot_values(&full_inner, data, item_size, items);
}
