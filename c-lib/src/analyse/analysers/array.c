/*

Analyse an array.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include "private/analyse.h"
#include "private/dotify.h"
#include "private/utility.h"



// Analyse an array from a pointer.
void _ctyan_ptrarray(_Cty_Die *die, void *data, size_t size)
{
    Dwarf_Attribute attr;
    Dwarf_Word word;

    dwarf_attr(&die->type, DW_AT_byte_size, &attr);
    dwarf_formudata(&attr, &word);

    Dwarf_Off off = dwarf_dieoffset(&die->qualifiers);


    // Analyse each element of the array.
    char *end = (char *)data + size;
    char *pos = data;
    
    for (; pos < end; pos += word)
        _ctyan_analyse_inline(off, pos);

    // Add the whole array into the interval container.
    // _ctycat_add_interval((uintptr_t)data, (uintptr_t)end, off);
}

// Analyse a fixed size array
void _ctyan_array(_Cty_Die *die, void *data)
{
    Dwarf_Attribute attr;
    Dwarf_Word word;
    Dwarf_Die inner_die;

    Dwarf_Die child; // subrange
    if (dwarf_child(&die->type, &child))
        return;

    dwarf_attr(&child, DW_AT_upper_bound, &attr);
    dwarf_formudata(&attr, &word);

    size_t items = word; 

    dwarf_attr(&die->type, DW_AT_type, &attr);
    dwarf_formref_die(&attr, &inner_die);

    dwarf_attr(&inner_die, DW_AT_byte_size, &attr);
    dwarf_formudata(&attr, &word);

    size_t item_size = word;

    // Analyse each element of the array.
    char *end = (char *)data + items * item_size;
    char *pos = data;

    Dwarf_Off off = dwarf_dieoffset(&die->qualifiers);
    
    for (; pos < end; pos += item_size)
        _ctyan_analyse_inline(off, pos);

    // Add the whole array into the interval container.
    // _ctycat_add_interval((uintptr_t)data, (uintptr_t)end, off);
}