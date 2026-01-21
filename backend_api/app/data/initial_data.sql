-- SI3LN Initial Data
-- Users

-- Admin User (password: admin1234 hashed with bcrypt)
INSERT INTO users (id, first_name, last_name, email, password, is_admin, created_at, updated_at)
VALUES (
    '36c9050e-ddd3-4c3b-9731-9f487208bbc1',
    'Admin', 
    'HBnB',
    'admin@hbnb.io',
    '$2b$12$LQv3c1yqBWVHxkd2g8a9HeYgJCYOWMtgzFdsBFk8Udmjr5qR4O.S6', -- admin1234
    TRUE,
    CURRENT_TIMESTAMP,
    CURRENT_TIMESTAMP
);

-- Sample regular user
INSERT INTO users (id, first_name, last_name, email, password, is_admin, created_at, updated_at)
VALUES (
    '47d8150f-eee4-4d4c-a842-0a598470abcd',
    'John',
    'Traveler',
    'john.traveler@example.com',
    '$2b$12$OtherHashExample1234567890ABCDEFGHIJK',
    FALSE,
    CURRENT_TIMESTAMP,
    CURRENT_TIMESTAMP
);
