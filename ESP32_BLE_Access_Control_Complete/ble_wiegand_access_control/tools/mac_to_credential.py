#!/usr/bin/env python3
"""
MAC Address to Access Control Credential Converter

This tool helps you convert BLE device MAC addresses to their corresponding
access control credentials for Wiegand 26-bit, Wiegand 34-bit, and EM4100 40-bit formats.

Use this to pre-calculate the credentials that will be sent to your access
control system so you can add them to your controller configuration.

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
        'format': 'Wiegand 26-bit',
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
        'format': 'Wiegand 34-bit',
        'mac_address': mac,
        'facility_code': facility_code,
        'card_number': card_number,
        'facility_code_hex': f'0x{facility_code:04X}',
        'card_number_hex': f'0x{card_number:04X}'
    }


def mac_to_em4100(mac_address):
    """
    Convert MAC address to 40-bit EM4100 format
    
    Args:
        mac_address: MAC address string
        
    Returns:
        Dictionary with version and card ID
    """
    # Normalize MAC
    mac = normalize_mac(mac_address)
    
    # Extract bytes
    mac_bytes = mac.replace(':', '')
    version = int(mac_bytes[0:2], 16)      # First byte
    card_id = int(mac_bytes[2:10], 16)     # Next 4 bytes (32 bits)
    
    return {
        'format': 'EM4100 40-bit',
        'mac_address': mac,
        'version': version,
        'card_id': card_id,
        'version_hex': f'0x{version:02X}',
        'card_id_hex': f'0x{card_id:08X}'
    }


def print_credential_info(cred_data):
    """
    Print formatted credential information
    
    Args:
        cred_data: Dictionary from conversion functions
    """
    print(f"\n{'='*60}")
    print(f"{cred_data['format']} Conversion")
    print(f"{'='*60}")
    print(f"MAC Address:     {cred_data['mac_address']}")
    
    if 'EM4100' in cred_data['format']:
        print(f"Version:         {cred_data['version']} ({cred_data['version_hex']})")
        print(f"Card ID:         {cred_data['card_id']} ({cred_data['card_id_hex']})")
    else:
        print(f"Facility Code:   {cred_data['facility_code']} ({cred_data['facility_code_hex']})")
        print(f"Card Number:     {cred_data['card_number']} ({cred_data['card_number_hex']})")
    
    print(f"{'='*60}\n")


def batch_convert(mac_addresses, format_type='all', facility_code=123):
    """
    Convert multiple MAC addresses
    
    Args:
        mac_addresses: List of MAC address strings
        format_type: '26', '34', '40', or 'all'
        facility_code: Facility code for 26-bit format
    """
    print("\n" + "="*60)
    print("BATCH CONVERSION RESULTS")
    print("="*60)
    
    for mac in mac_addresses:
        try:
            if format_type in ['26', 'all']:
                data_26 = mac_to_wiegand_26(mac, facility_code)
                print(f"\nMAC: {data_26['mac_address']}")
                print(f"  W26: FC={data_26['facility_code']}, CN={data_26['card_number']}")
            
            if format_type in ['34', 'all']:
                data_34 = mac_to_wiegand_34(mac)
                if format_type == 'all':
                    print(f"  W34: FC={data_34['facility_code']}, CN={data_34['card_number']}")
                else:
                    print(f"\nMAC: {data_34['mac_address']}")
                    print(f"  W34: FC={data_34['facility_code']}, CN={data_34['card_number']}")
            
            if format_type in ['40', 'all']:
                data_40 = mac_to_em4100(mac)
                if format_type == 'all':
                    print(f"  EM4100: Ver={data_40['version']}, ID={data_40['card_id']}")
                else:
                    print(f"\nMAC: {data_40['mac_address']}")
                    print(f"  EM4100: Ver={data_40['version']}, ID={data_40['card_id']}")
                    
        except ValueError as e:
            print(f"\nERROR processing {mac}: {e}")
    
    print("\n" + "="*60 + "\n")


def generate_csv(mac_addresses, output_file, format_type='all', facility_code=123):
    """
    Generate CSV file with MAC to credential mappings
    
    Args:
        mac_addresses: List of MAC address strings
        output_file: Output CSV filename
        format_type: '26', '34', '40', or 'all'
        facility_code: Facility code for 26-bit format
    """
    import csv
    
    with open(output_file, 'w', newline='') as csvfile:
        if format_type == 'all':
            fieldnames = ['MAC Address', 
                         'W26 Facility Code', 'W26 Card Number',
                         'W34 Facility Code', 'W34 Card Number',
                         'EM4100 Version', 'EM4100 Card ID']
        elif format_type == '26':
            fieldnames = ['MAC Address', 'Facility Code', 'Card Number']
        elif format_type == '34':
            fieldnames = ['MAC Address', 'Facility Code', 'Card Number']
        elif format_type == '40':
            fieldnames = ['MAC Address', 'Version', 'Card ID']
        
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        writer.writeheader()
        
        for mac in mac_addresses:
            try:
                row = {'MAC Address': normalize_mac(mac)}
                
                if format_type in ['26', 'all']:
                    data_26 = mac_to_wiegand_26(mac, facility_code)
                    if format_type == 'all':
                        row['W26 Facility Code'] = data_26['facility_code']
                        row['W26 Card Number'] = data_26['card_number']
                    else:
                        row['Facility Code'] = data_26['facility_code']
                        row['Card Number'] = data_26['card_number']
                
                if format_type in ['34', 'all']:
                    data_34 = mac_to_wiegand_34(mac)
                    if format_type == 'all':
                        row['W34 Facility Code'] = data_34['facility_code']
                        row['W34 Card Number'] = data_34['card_number']
                    else:
                        row['Facility Code'] = data_34['facility_code']
                        row['Card Number'] = data_34['card_number']
                
                if format_type in ['40', 'all']:
                    data_40 = mac_to_em4100(mac)
                    if format_type == 'all':
                        row['EM4100 Version'] = data_40['version']
                        row['EM4100 Card ID'] = data_40['card_id']
                    else:
                        row['Version'] = data_40['version']
                        row['Card ID'] = data_40['card_id']
                
                writer.writerow(row)
                
            except ValueError as e:
                print(f"ERROR processing {mac}: {e}")
    
    print(f"\nCSV file generated: {output_file}\n")


def main():
    """Main function for command-line usage"""
    import argparse
    
    parser = argparse.ArgumentParser(
        description='Convert BLE MAC addresses to access control credentials',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  # Convert single MAC to all formats
  python mac_to_credential.py -m AA:BB:CC:DD:EE:FF
  
  # Convert single MAC to EM4100 only
  python mac_to_credential.py -m AA:BB:CC:DD:EE:FF -f 40
  
  # Batch convert from file
  python mac_to_credential.py -i mac_list.txt -f all
  
  # Generate CSV output
  python mac_to_credential.py -i mac_list.txt -o credentials.csv
        """
    )
    
    parser.add_argument('-m', '--mac', help='Single MAC address to convert')
    parser.add_argument('-i', '--input', help='Input file with MAC addresses (one per line)')
    parser.add_argument('-o', '--output', help='Output CSV file')
    parser.add_argument('-f', '--format', choices=['26', '34', '40', 'all'], default='all',
                       help='Output format: 26=Wiegand26, 34=Wiegand34, 40=EM4100, all=All formats (default: all)')
    parser.add_argument('-c', '--facility-code', type=int, default=123,
                       help='Facility code for 26-bit Wiegand format (default: 123)')
    
    args = parser.parse_args()
    
    # Validate facility code
    if not 0 <= args.facility_code <= 255:
        print(f"ERROR: Facility code must be 0-255, got {args.facility_code}")
        sys.exit(1)
    
    # Single MAC conversion
    if args.mac:
        try:
            if args.format in ['26', 'all']:
                data_26 = mac_to_wiegand_26(args.mac, args.facility_code)
                print_credential_info(data_26)
            
            if args.format in ['34', 'all']:
                data_34 = mac_to_wiegand_34(args.mac)
                print_credential_info(data_34)
            
            if args.format in ['40', 'all']:
                data_40 = mac_to_em4100(args.mac)
                print_credential_info(data_40)
                
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

