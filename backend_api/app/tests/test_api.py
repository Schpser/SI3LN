import requests
import json
import os
import sys

API_URL = "http://localhost:5000/api/v1"
PLACEHOLDER_ID = "3fa85f64-5717-4562-b3fc-2c963f66afa6"

class Colors:
    RED = '\033[0;31m'
    GREEN = '\033[0;32m'
    YELLOW = '\033[1;33m'
    BLUE = '\033[0;34m'
    NC = '\033[0m'

IDS = {
    'user': None,
    'amenity': None,
    'place': None,
    'review': None,
}

def print_step(message):
    """Display a test step."""
    print(f"\n{Colors.BLUE}▶ {message}{Colors.NC}")
    print("-" * (len(message) + 4))

def print_success(message):
    """Display a success message."""
    print(f"{Colors.GREEN}✅ {message}{Colors.NC}")

def print_error(message, fatal=False):
    """Display an error message and exit if fatal."""
    print(f"{Colors.RED}❌ {message}{Colors.NC}")
    if fatal:
        sys.exit(1)

def print_info(message):
    """Display an information message."""
    print(f"{Colors.YELLOW}ℹ️  {message}{Colors.NC}")

def extract_id(response_json, resource_name, fatal=False):
    """Attempt to extract ID from a JSON response."""
    try:
        response_dict = response_json
        resource_id = response_dict.get('id')
        if resource_id:
            print_success(f"{resource_name.capitalize()} ID: {resource_id}")
            return resource_id
        else:
            print_error(f"Critical failure: {resource_name} response does not contain an ID.", fatal=fatal)
            return None
    except Exception as e:
        print_error(f"ID extraction error for {resource_name}: {e}", fatal=fatal)
        return None

def make_request(method, endpoint, data=None):
    """Execute an HTTP request and return the response object."""
    url = f"{API_URL}/{endpoint}"
    headers = {'Content-Type': 'application/json'}
    
    try:
        if method == 'GET':
            response = requests.get(url, headers=headers)
        elif method == 'POST':
            response = requests.post(url, headers=headers, data=json.dumps(data) if data else None)
        elif method == 'PUT':
            response = requests.put(url, headers=headers, data=json.dumps(data) if data else None)
        elif method == 'DELETE':
            response = requests.delete(url, headers=headers)
        else:
            raise ValueError(f"Unsupported HTTP method: {method}")
        
        print_info(f"{method} {endpoint} -> Status: {response.status_code}")
        try:
            response_json = response.json()
            print(json.dumps(response_json, indent=4, ensure_ascii=False))
            return response_json
        except requests.exceptions.JSONDecodeError:
            print(f"Raw response: {response.text}")
            return None
        
    except requests.exceptions.ConnectionError:
        print_error(f"Connection failed to {url}. Make sure the API is running.", fatal=True)
        return None
    except Exception as e:
        print_error(f"An unexpected error occurred during the request: {e}", fatal=False)
        return None

def run_tests():
    """Execute the API test sequence."""
    print("====================================================================")
    print("🚀 HBNB API TEST - ROBUST PYTHON SCRIPT")
    print("====================================================================")

    print_step("1. TEST USERS")
    
    print("Creating user (John Doe)...")
    user_data = {"first_name": "John", "last_name": "Doe", "email": "john.doe@example.com"}
    user_response = make_request('POST', 'users/', user_data)
    
    user_id = extract_id(user_response, 'user', fatal=True)
    IDS['user'] = user_id
    
    print("\nGET user by ID (John)...")
    make_request('GET', f'users/{user_id}')
    
    print("\nGET all users...")
    make_request('GET', 'users/')
    
    print("\nUPDATE user (to Jane Doe)...")
    user_update_data = {"first_name": "Jane", "last_name": "Doe", "email": "jane.doe@example.com"}
    make_request('PUT', f'users/{user_id}', user_update_data)

    print_step("2. TEST AMENITIES")
    
    print("Creating amenity 1 (Wi-Fi)...")
    am1_data = {"name": "Wi-Fi"}
    am1_response = make_request('POST', 'amenities/', am1_data)
    
    am1_id = extract_id(am1_response, 'amenity', fatal=True)
    IDS['amenity'] = am1_id

    print("\nGET all amenities...")
    make_request('GET', 'amenities/')
    
    print("\nGET amenity by ID (Wi-Fi)...")
    make_request('GET', f'amenities/{am1_id}')
    
    print("\nUPDATE amenity (to Air Conditioning)...")
    am_update_data = {"name": "Air Conditioning"}
    make_request('PUT', f'amenities/{am1_id}', am_update_data)

    print_step("3. TEST PLACES")
    
    print("Creating place (Cozy Apartment)...")

    VALID_CITY_ID = "00000000-0000-0000-0000-000000000001"

    place_data = {
    
      "title": "Cozy Apartment",
      "description": "A nice place to stay",
      "price": 100.0,
      "latitude": 37.7749,
      "longitude": -122.4194,
      "owner_id": IDS['user'],
      "amenities": [],
    }
    
    place_response = make_request('POST', 'places/', place_data)
    
    place_id = extract_id(place_response, 'place', fatal=True)
    IDS['place'] = place_id

    print("\nGET all places...")
    make_request('GET', 'places/')

    print("\nGET place by ID (Cozy Apartment)...")
    make_request('GET', f'places/{place_id}')
    
    print("\nUPDATE place (to Luxury Condo)...")
    place_update_data = {"title": "Luxury Condo", "description": "An upscale place to stay", "price": 200.0}
    make_request('PUT', f'places/{place_id}', place_update_data)

    print_step("4. TEST REVIEWS")
    
    print("Creating review (Great place to stay!)...")
    review_data = {
        "text": "Great place to stay!",
        "rating": 5,
        "user_id": IDS['user'],
        "place_id": IDS['place']
    }
    
    review_response = make_request('POST', 'reviews/', review_data)
    
    review_id = extract_id(review_response, 'review')
    if not review_id:
        print_error("Review creation failed. Subsequent tests might fail.")
        review_id = PLACEHOLDER_ID
    IDS['review'] = review_id

    print("\nGET all reviews...")
    make_request('GET', 'reviews/')
    
    print("\nGET review by ID (Great place to stay!)...")
    make_request('GET', f'reviews/{review_id}')
    
    print("\nUPDATE review (to Amazing stay!)...")
    review_update_data = {"text": "Amazing stay!", "rating": 4}
    make_request('PUT', f'reviews/{review_id}', review_update_data)

    print("\nGET reviews by place...")
    make_request('GET', f'places/{IDS["place"]}/reviews')
    
    print("\nDELETE review...")
    make_request('DELETE', f'reviews/{review_id}')

    print_step("📊 FINAL REPORT")
    print_success("Tests completed. Check 200/201 status codes for success.")
    
    print("\n====================================================================")
    print_info("IDs used in tests:")
    print_info(f"User: {IDS.get('user', 'Not created')}")
    print_info(f"Amenity: {IDS.get('amenity', 'Not created')}")
    print_info(f"Place: {IDS.get('place', 'Not created')}")
    print_info(f"Review: {IDS.get('review', 'Not created')}")
    print("====================================================================")

if __name__ == "__main__":
    run_tests()
