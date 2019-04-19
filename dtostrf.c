/*
 *
 *    This is a dtostrf implementation specially aimed at Arduino or general embedded programming, 
 *    whith a small code footprint compared to other implementations which make use of stdio libraries
 *
 *    Copyright (C) 2019  Paolo Malaspina, https://www.linkedin.com/in/paolomalaspina/
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <https://www.gnu.org/licenses/>. 
 * 
 */

/*
 * Print float to char array, return the pointer to the start of the string
 *  
 */

char *my_dtostrf( float val,  int8_t char_num, uint8_t precision, char *chr_buffer)
{
  int       right_j;
  int       i, j ;
  float     r_val;
  long      i_val;
  char      c, c_sign;


  // check the sign
  if (val < 0.0) {
    // print the - sign
    c_sign = '-';

    // process the absolute value
    val = - val;
  } else {
    // put a space for positive numbers
    c_sign = ' ';
    
  }

  // check the left-right justification
  if (char_num < 0)
  {
    // set the flag
    right_j = 1;

    // make the number positive
    char_num = -char_num; 
    
  } else {
    right_j = 0;
  }


  // no native exponential function for int
  j=1;
  for(i=0; i < (char_num - precision - 3 );i++) j *= 10;
  
  // Hackish fail-fast behavior for larger-than-what-can-be-printed values, countig the precision + sign ('-') +'.' + '\0'
  if (val >= (float)(j))
  {
    // not enough space
    // strcpy(chr_buffer, "ovf"); - this is very byte consuming (388 bytes) , so we go for the cheap array
    chr_buffer[0] = 'o';
    chr_buffer[1] = 'v';
    chr_buffer[2] = 'f';
    chr_buffer[3] = '\0';

    // finish here
    return chr_buffer;
  }  



  // Simplistic rounding strategy so that e.g. print(1.999, 2)
  // prints as "2.00"
  r_val = 0.5;
  for ( i = 0; i < precision; i++) {
      // debug
      // Serial.println(r_val, 6);
      r_val /= 10.0;
  }
  val += r_val;

  // Extract the integer and decimal part of the number and print it
  i_val = (long) val;
  r_val = val - (float) i_val;


  // print the integral part ... but it is in reverse order ... so leaves the space for  '.' and the decimal part (and remember that array indexes start from 0
  i = char_num - precision - 2;
  do 
  {
      // debug
      // Serial.println(i_val);
      
      chr_buffer[i] = '0' + ( i_val % 10);
      i_val /= 10;
      i--;
      
  }   while ( i_val > 0) ;

  // add the sign char
  chr_buffer[i] = c_sign;

  // prepare for the decimal part
  j = char_num - precision - 1;

  // Print the decimal point, but only if there are digits beyond
  if (precision > 0) {
    chr_buffer[j] = '.';
    j++;

    // Extract digits from the remainder one at a time
    while (precision > 0) {
      // prepare the data
      r_val *= 10.0;
      i_val  = (int)    r_val;
      r_val -= (float)  i_val;
  
      // update the string
      chr_buffer[j] = '0' + ( i_val );
      j++;

      // use precision as the counter
      precision--;      
    }
  }

  // terminate the string
  chr_buffer[j] = '\0';

  // check the justification direction
  if (right_j)
  {
    // pad the string with leading ' '
    while (i > 0)
    {
      i--;

      chr_buffer[i] = ' ';       
    }
    
  } 
  
 // return the pointer to the first char of the prepared string
 return ( &chr_buffer[i]); 

}
