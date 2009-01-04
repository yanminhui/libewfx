/*
 * libewf segment table
 *
 * Copyright (c) 2006-2007, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of the creator, related organisations, nor the names of
 *   its contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER, COMPANY AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "libewf_includes.h"

#include <libewf/libewf_definitions.h>

#include "libewf_common.h"
#include "libewf_notify.h"
#include "libewf_segment_table.h"
#include "libewf_string.h"

/* Allocates memory for a segment table struct
 * Returns a pointer to the new instance, NULL on error
 */
LIBEWF_SEGMENT_TABLE *libewf_segment_table_alloc( uint16_t amount )
{
	LIBEWF_SEGMENT_TABLE *segment_table = NULL;
	static char *function               = "libewf_segment_table_alloc";
	uint16_t iterator                   = 0;

	segment_table = (LIBEWF_SEGMENT_TABLE *) libewf_common_alloc( LIBEWF_SEGMENT_TABLE_SIZE );

	if( segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate segment table.\n",
		 function );

		return( NULL );
	}
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
	segment_table->filename = (wchar_t **) libewf_common_alloc_cleared(
	                           ( amount * LIBEWF_SEGMENT_TABLE_FILENAME_SIZE ),
	                           0 );
#else
	segment_table->filename = (char **) libewf_common_alloc_cleared(
	                           ( amount * LIBEWF_SEGMENT_TABLE_FILENAME_SIZE ),
	                           0 );
#endif

	if( segment_table->filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate filename array.\n",
		 function );

		libewf_common_free( segment_table );

		return( NULL );
	}
	segment_table->file_descriptor = (int *) libewf_common_alloc_cleared(
	                                  ( amount * LIBEWF_SEGMENT_TABLE_FILE_DESCRIPTOR_SIZE ),
	                                  -1 );

	if( segment_table->file_descriptor == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate file descriptor array.\n",
		 function );

		libewf_common_free( segment_table->filename );
		libewf_common_free( segment_table );

		return( NULL );
	}
	segment_table->file_offset = (off64_t *) libewf_common_alloc_cleared(
	                              ( amount * LIBEWF_SEGMENT_TABLE_FILE_OFFSET_SIZE ),
	                              0 );

	if( segment_table->file_offset == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate file offset array.\n",
		 function );

		libewf_common_free( segment_table->filename );
		libewf_common_free( segment_table->file_descriptor );
		libewf_common_free( segment_table );

		return( NULL );
	}
	segment_table->amount_of_chunks = (uint32_t *) libewf_common_alloc_cleared(
	                                   ( amount * LIBEWF_SEGMENT_TABLE_AMOUNT_OF_CHUNKS_SIZE ),
	                                   0 );

	if( segment_table->amount_of_chunks == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate amount of chunks array.\n",
		 function );

		libewf_common_free( segment_table->filename );
		libewf_common_free( segment_table->file_descriptor );
		libewf_common_free( segment_table->file_offset );
		libewf_common_free( segment_table );

		return( NULL );
	}
	segment_table->section_list = (LIBEWF_SECTION_LIST **) libewf_common_alloc_cleared(
	                               ( amount * LIBEWF_SEGMENT_TABLE_SECTION_LIST_SIZE ),
	                               0 );

	if( segment_table->section_list == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to allocate section list array.\n",
		 function );

		libewf_common_free( segment_table->filename );
		libewf_common_free( segment_table->file_descriptor );
		libewf_common_free( segment_table->file_offset );
		libewf_common_free( segment_table->amount_of_chunks );
		libewf_common_free( segment_table );

		return( NULL );
	}
	for( iterator = 0; iterator < amount; iterator++ )
	{
		if( segment_table->section_list[ iterator ] == NULL )
		{
			segment_table->section_list[ iterator ] = (LIBEWF_SECTION_LIST *) libewf_common_alloc( LIBEWF_SECTION_LIST_SIZE );

			if( segment_table->section_list[ iterator ] == NULL )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to allocate section list.\n",
				 function );

				/* The current entry does not need to be freed, because it was never allocated
				 * but the first entry 0 does, because the iterator is a unsigned integer
				 * the iterator - 1 construction is used
				 * There are no entries in the list
				 */
				for( ; iterator > 0; iterator-- )
				{
					libewf_common_free( segment_table->section_list[ iterator - 1 ] );
				}
				libewf_common_free( segment_table->filename );
				libewf_common_free( segment_table->file_descriptor );
				libewf_common_free( segment_table->file_offset );
				libewf_common_free( segment_table->amount_of_chunks );
				libewf_common_free( segment_table->section_list );
				libewf_common_free( segment_table );

				return( NULL );
			}
			segment_table->section_list[ iterator ]->first = NULL;
			segment_table->section_list[ iterator ]->last  = NULL;
		}
		else
		{
			LIBEWF_WARNING_PRINT( "%s: section list already created.\n",
			 function );
		}
	}
	segment_table->amount = amount;

	return( segment_table );
}

/* Reallocates memory for the segment table values
 * Returns 1 if successful, or -1 on error
 */
int libewf_segment_table_realloc( LIBEWF_SEGMENT_TABLE *segment_table, uint16_t amount )
{
	void *reallocation    = NULL;
	static char *function = "libewf_segment_table_realloc";
	uint16_t iterator     = 0;

	if( segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment table.\n",
		 function );

		return( -1 );
	}
	reallocation = libewf_common_realloc_new_cleared(
	                segment_table->filename,
	                ( segment_table->amount * LIBEWF_SEGMENT_TABLE_FILENAME_SIZE ),
	                ( amount * LIBEWF_SEGMENT_TABLE_FILENAME_SIZE ),
	                0 );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to reallocate dynamic filename array.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
	segment_table->filename = (wchar_t **) reallocation;
#else
	segment_table->filename = (char **) reallocation;
#endif
	reallocation            = libewf_common_realloc_new_cleared(
	                           segment_table->file_descriptor,
	                           ( segment_table->amount * LIBEWF_SEGMENT_TABLE_FILE_DESCRIPTOR_SIZE ),
	                           ( amount * LIBEWF_SEGMENT_TABLE_FILE_DESCRIPTOR_SIZE ),
	                           -1 );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to reallocate dynamic file descriptor array.\n",
		 function );

		return( -1 );
	}
	segment_table->file_descriptor = (int *) reallocation;
	reallocation                   = libewf_common_realloc_new_cleared(
	                                  segment_table->file_offset,
	                                  ( segment_table->amount * LIBEWF_SEGMENT_TABLE_FILE_OFFSET_SIZE ),
	                                  ( amount * LIBEWF_SEGMENT_TABLE_FILE_OFFSET_SIZE ),
	                                  0 );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to reallocate dynamic file offset array.\n",
		 function );

		return( -1 );
	}
	segment_table->file_offset = (off64_t *) reallocation;
	reallocation               = libewf_common_realloc_new_cleared(
	                              segment_table->amount_of_chunks,
	                              ( segment_table->amount * LIBEWF_SEGMENT_TABLE_AMOUNT_OF_CHUNKS_SIZE ),
	                              ( amount * LIBEWF_SEGMENT_TABLE_AMOUNT_OF_CHUNKS_SIZE ),
	                              0 );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to reallocate dynamic amount of chunks array.\n",
		 function );

		return( -1 );
	}
	segment_table->amount_of_chunks = (uint32_t *) reallocation;
	reallocation                    = libewf_common_realloc_new_cleared(
	                                   segment_table->section_list,
	                                   ( segment_table->amount * LIBEWF_SEGMENT_TABLE_SECTION_LIST_SIZE ),
	                                   ( amount * LIBEWF_SEGMENT_TABLE_SECTION_LIST_SIZE ),
	                                   0 );

	if( reallocation == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to reallocate dynamic section list array.\n",
		 function );

		return( -1 );
	}
	segment_table->section_list = (LIBEWF_SECTION_LIST **) reallocation;

	for( iterator = segment_table->amount; iterator < amount; iterator++ )
	{
		if( segment_table->section_list[ iterator ] == NULL )
		{
			segment_table->section_list[ iterator ] = (LIBEWF_SECTION_LIST *) libewf_common_alloc( LIBEWF_SECTION_LIST_SIZE );

			if( segment_table->section_list[ iterator ] == NULL )
			{
				LIBEWF_WARNING_PRINT( "%s: unable to allocate section list.\n",
				 function );

				return( -1 );
			}
			segment_table->section_list[ iterator ]->first = NULL;
			segment_table->section_list[ iterator ]->last  = NULL;
		}
		else
		{
			LIBEWF_WARNING_PRINT( "%s: section list already created.\n",
			 function );
		}
	}
	segment_table->amount = amount;

	return( 1 );
}

/* Frees memory of a file list struct including elements
 */
void libewf_segment_table_free( LIBEWF_SEGMENT_TABLE *segment_table )
{
	LIBEWF_SECTION_LIST_ENTRY *section_list_entry         = NULL;
	LIBEWF_SECTION_LIST_ENTRY *current_section_list_entry = NULL;
	static char *function                                 = "libewf_segment_table_free";
	uint16_t iterator                                     = 0;

	if( segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment table.\n",
		 function );

		return;
	}
	for( iterator = 0; iterator < segment_table->amount; iterator++ )
	{
		if( segment_table->filename[ iterator ] != NULL )
		{
			libewf_common_free( segment_table->filename[ iterator ] );
		}
		if( segment_table->section_list[ iterator ] != NULL )
		{
			section_list_entry = segment_table->section_list[ iterator ]->first;

			while( section_list_entry != NULL )
			{
				current_section_list_entry = section_list_entry;
				section_list_entry         = section_list_entry->next;

				libewf_common_free( current_section_list_entry );
			}
			libewf_common_free( segment_table->section_list[ iterator ] );
		}
	}
	libewf_common_free( segment_table->filename );
	libewf_common_free( segment_table->file_descriptor );
	libewf_common_free( segment_table->file_offset );
	libewf_common_free( segment_table->amount_of_chunks );
	libewf_common_free( segment_table->section_list );
	libewf_common_free( segment_table );
}

/* Gets a filename of a certain segment
 * Returns the filename pointer, or NULL on error
 */
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
wchar_t *libewf_segment_table_get_wide_filename( LIBEWF_SEGMENT_TABLE *segment_table, uint16_t segment )
#else
char *libewf_segment_table_get_filename( LIBEWF_SEGMENT_TABLE *segment_table, uint16_t segment )
#endif
{
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
	static char *function = "libewf_segment_table_get_wide_filename";
#else
	static char *function = "libewf_segment_table_get_filename";
#endif

	if( segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment table.\n",
		 function );

		return( NULL );
	}
	if( segment > segment_table->amount )
	{
		LIBEWF_WARNING_PRINT( "%s: segment out of range.\n",
		 function );

		return( NULL );
	}
	return( segment_table->filename[ segment ] );
}

/* Sets a filename for a specific segment
 * Creates a duplicate of the string
 * Returns 1 if succesful, or -1 on error
 */
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
int libewf_segment_table_set_wide_filename( LIBEWF_SEGMENT_TABLE *segment_table, uint16_t segment, const wchar_t *filename, size_t length_filename )
#else
int libewf_segment_table_set_filename( LIBEWF_SEGMENT_TABLE *segment_table, uint16_t segment, const char *filename, size_t length_filename )
#endif
{
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
	static char *function = "libewf_segment_table_set_wide_filename";
#else
	static char *function = "libewf_segment_table_set_filename";
#endif

	if( segment_table == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid segment table.\n",
		 function );

		return( -1 );
	}
	if( filename == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid filename.\n",
		 function );

		return( -1 );
	}
	if( segment > segment_table->amount )
	{
		LIBEWF_WARNING_PRINT( "%s: segment out of range.\n",
		 function );

		return( -1 );
	}
	if( segment_table->filename[ segment ] != NULL )
	{
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
		LIBEWF_WARNING_PRINT( "%s: duplicate segments not supported: segment %d in %ls was already specified as %ls.\n",
		 function, segment, filename, segment_table->filename[ segment ] );
#else
		LIBEWF_WARNING_PRINT( "%s: duplicate segments not supported: segment %d in %s was already specified as %s.\n",
		 function, segment, filename, segment_table->filename[ segment ] );
#endif

		return( -1 );
	}
	if( length_filename == 0 )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid filename length is zero.\n",
		 function );

		return( -1 );
	}
	if( length_filename >= (size_t) SSIZE_MAX )
	{
		LIBEWF_WARNING_PRINT( "%s: invalid filename length value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	/* One additional byte for the end of string character is needed
	 */
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
	segment_table->filename[ segment ] = (wchar_t *) libewf_common_alloc( sizeof( wchar_t ) * ( length_filename + 1 ) );
#else
	segment_table->filename[ segment ] = (char *) libewf_common_alloc( sizeof( char ) * ( length_filename + 1 ) );
#endif

	if( segment_table->filename[ segment ] == NULL )
	{
		LIBEWF_WARNING_PRINT( "%s: unable to create filename.\n",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_CHARACTER_TYPE ) && defined( HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS )
	if( libewf_common_wide_memcpy( segment_table->filename[ segment ], filename, length_filename ) == NULL )
#else
	if( libewf_common_memcpy( segment_table->filename[ segment ], filename, length_filename ) == NULL )
#endif
	{
		LIBEWF_WARNING_PRINT( "%s: unable to set filename.\n",
		 function );

		libewf_common_free( segment_table->filename[ segment ] );

		segment_table->filename[ segment ] = NULL;

		return( -1 );
	}
	/* Make sure the string is terminated
	 */
	segment_table->filename[ segment ][ length_filename ] = '\0';

	return( 1 );
}

