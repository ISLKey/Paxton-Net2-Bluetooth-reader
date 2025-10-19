#!/usr/bin/env python3
"""
MAC Address to Wiegand Converter

This tool helps you convert BLE device MAC addresses to their corresponding
Wiegand card numbers and facility codes for both 26-bit and 34-bit formats.

Use this to pre-calculate the credentials that will be sent to your Paxton
Net2 Plus controller so you can add them to your access control system.

Author: Manus AI
Date: October 2025
"""

import sys
import re


def normalize_mac(mac_address):
    """
    Normalize MAC address to standard format
    
    Args:
        mac_address: MAC address in various formats
        
    Returns:
        Normalized MAC address (uppercase with colons)
    """
    # Remove common separators
    mac = mac_address.replace(':', '').replace('-', '').replace('.', '').upper()
    
    # Validate length
    if len(mac) != 12:
        raise ValueError(f"Invalid MAC address length: {mac_address}")
    
    # Validate hex characters
    if not all(c in '0123456789ABCDEF' for c in mac):
        raise ValueError(f"Invalid MAC address characters: {mac_address}")
    
    # Format with colons
    return ':'.join([mac[i:i+2] for i in range(0, 12, 2)])


def mac_to_wiegand_26(mac_address, facility_code=123):
    """
    Convert MAC address to 26-bit Wiegand format
    
    Args:
        mac_address: MAC address string
        facility_code: 8-bit facility code (0-255)
        
    Returns:
        Dictionary with facility code and card number
    """
    # Normalize MAC
    mac = normalize_mac(mac_address)
    
    # Extract last 2 bytes for card number
    mac_bytes = mac.replace(':', '')
    card_number = int(mac_bytes[-4:], 16)  # Last 4 hex chars = 2 bytes
    
    # Validate facility code
    if not 0 <= facility_code <= 255:
        raise ValueError(f"Facility code must be 0-255, got {facility_code}")
    
    return {
        'format': '26-bit',
        'mac_address': mac,
        'facility_code': facility_code,
        'card_number': card_number,
        'facility_code_hex': f'0x{facility_code:02X}',
        'card_number_hex': f'0x{card_number:04X}'
    }


def mac_to_wiegand_34(mac_address):
    """
    Convert MAC address to 34-bit Wiegand format
    
    Args:
        mac_address: MAC address string
        
    Returns:
        Dictionary with facility code and card number
    """
    # Normalize MAC
    mac = normalize_mac(mac_address)
    
    # Extract bytes for facility code and card number
    mac_bytes = mac.replace(':', '')
    facility_code = int(mac_bytes[4:8], 16)  # Bytes 3-4
    card_number = int(mac_bytes[-4:], 16)    # Last 2 bytes
    
    return {
        'format': '34-bit',
        'mac_address': mac,
        'facility_code': facility_code,
        'card_number': card_number,
        'facility_code_hex': f'0x{facility_code:04X}',
        'card_number_hex': f'0x{card_number:04X}'
    }


def print_wiegand_info(wiegand_data):
    """
    Print formatted Wiegand information
    
    Args:
        wiegand_data: Dictionary from mac_to_wiegand_26 or mac_to_wiegand_34
    """
    print(f"\n{'='*60}")
    print(f"Wiegand {wiegand_data['format']} Conversion")
    print(f"{'='*60}")
    print(f"MAC Address:     {wiegand_data['mac_address']}")
    print(f"Facility Code:   {wiegand_data['facility_code']} ({wiegand_data['facility_code_hex']})")
    print(f"Card Number:     {wiegand_data['card_number']} ({wiegand_data['card_number_hex']})")
    print(f"{'='*60}\n")


def batch_convert(mac_addresses, format_type='both', facility_code=123):
    """
    Convert multiple MAC addresses
    
    Args:
        mac_addresses: List of MAC address strings
        format_type: '26', '34', or 'both'
        facility_code: Facility code for 26-bit format
    """
    print("\n" + "="*60)
    print("BATCH CONVERSION RESULTS")
    print("="*60)
    
    for mac in mac_addresses:
        try:
            if format_type in ['26', 'both']:
                data_26 = mac_to_wiegand_26(mac, facility_code)
                print(f"\nMAC: {data_26['mac_address']}")
                print(f"  26-bit: FC={data_26['facility_code']}, CN={data_26['card_number']}")
            
            if format_type in ['34', 'both']:
                data_34 = mac_to_wiegand_34(mac)
                if format_type == 'both':
                    print(f"  34-bit: FC={data_34['facility_code']}, CN={data_34['card_number']}")
                else:
                    print(f"\nMAC: {data_34['mac_address']}")
                    print(f"  34-bit: FC={data_34['facility_code']}, CN={data_34['card_number']}")
                    
        except ValueError as e:
            print(f"\nERROR processing {mac}: {e}")
    
    print("\n" + "="*60 + "\n")


def generate_csv(mac_addresses, output_file, format_type='both', facility_code=123):
    """
    Generate CSV file with MAC to Wiegand mappings
    
    Args:
        mac_addresses: List of MAC address strings
        output_file: Output CSV filename
        format_type: '26', '34', or 'both'
        facility_code: Facility code for 26-bit format
    """
    import csv
    
    with open(output_file, 'w', newline='') as csvfile:
        if format_type == 'both':
            fieldnames = ['MAC Address', '26-bit Facility Code', '26-bit Card Number', 
                         '34-bit Facility Code', '34-bit Card Number']
        elif format_type == '26':
            fieldnames = ['MAC Address', 'Facility Code', 'Card Number']
        else:
            fieldnames = ['MAC Address', 'Facility Code', 'Card Number']
        
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        writer.writeheader()
        
        for mac in mac_addresses:
            try:
                row = {'MAC Address': normalize_mac(mac)}
                
                if format_type in ['26', 'both']:
                    data_26 = mac_to_wiegand_26(mac, facility_code)
                    if format_type == 'both':
                        row['26-bit Facility Code'] = data_26['facility_code']
                        row['26-bit Card Number'] = data_26['card_number']
                    else:
                        row['Facility Code'] = data_26['facility_code']
                        row['Card Number'] = data_26['card_number']
                
                if format_type in ['34', 'both']:
                    data_34 = mac_to_wiegand_34(mac)
                    if format_type == 'both':
                        row['34-bit Facility Code'] = data_34['facility_code']
                        row['34-bit Card Number'] = data_34['card_number']
                    else:
                        row['Facility Code'] = data_34['facility_code']
                        row['Card Number'] = data_34['card_number']
                
                writer.writerow(row)
                
            except ValueError as e:
                print(f"ERROR processing {mac}: {e}")
    
    print(f"\nCSV file generated: {output_file}\n")


def main():
    """Main function for command-line usage"""
    import argparse
    
    parser = argparse.ArgumentParser(
        description='Convert BLE MAC addresses to Wiegand credentials',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  # Convert single MAC to 26-bit
  python mac_to_wiegand.py -m AA:BB:CC:DD:EE:FF -f 26 -c 123
  
  # Convert single MAC to both formats
  python mac_to_wiegand.py -m AA:BB:CC:DD:EE:FF
  
  # Batch convert from file
  python mac_to_wiegand.py -i mac_list.txt -f both
  
  # Generate CSV output
  python mac_to_wiegand.py -i mac_list.txt -o credentials.csv
        """
    )
    
    parser.add_argument('-m', '--mac', help='Single MAC address to convert')
    parser.add_argument('-i', '--input', help='Input file with MAC addresses (one per line)')
    parser.add_argument('-o', '--output', help='Output CSV file')
    parser.add_argument('-f', '--format', choices=['26', '34', 'both'], default='both',
                       help='Wiegand format (default: both)')
    parser.add_argument('-c', '--facility-code', type=int, default=123,
                       help='Facility code for 26-bit format (default: 123)')
    
    args = parser.parse_args()
    
    # Validate facility code
    if not 0 <= args.facility_code <= 255:
        print(f"ERROR: Facility code must be 0-255, got {args.facility_code}")
        sys.exit(1)
    
    # Single MAC conversion
    if args.mac:
        try:
            if args.format in ['26', 'both']:
                data_26 = mac_to_wiegand_26(args.mac, args.facility_code)
                print_wiegand_info(data_26)
            
            if args.format in ['34', 'both']:
                data_34 = mac_to_wiegand_34(args.mac)
                print_wiegand_info(data_34)
                
        except ValueError as e:
            print(f"ERROR: {e}")
            sys.exit(1)
    
    # Batch conversion from file
    elif args.input:
        try:
            with open(args.input, 'r') as f:
                mac_addresses = [line.strip() for line in f if line.strip() and not line.startswith('#')]
            
            if args.output:
                generate_csv(mac_addresses, args.output, args.format, args.facility_code)
            else:
                batch_convert(mac_addresses, args.format, args.facility_code)
                
        except FileNotFoundError:
            print(f"ERROR: File not found: {args.input}")
            sys.exit(1)
        except Exception as e:
            print(f"ERROR: {e}")
            sys.exit(1)
    
    else:
        parser.print_help()
        sys.exit(1)


if __name__ == '__main__':
    main()

