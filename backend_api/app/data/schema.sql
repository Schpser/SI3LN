-- Database Schema for SI3LN Game Backend

-- Users table
CREATE TABLE IF NOT EXISTS users (
    id CHAR(36) PRIMARY KEY,
    first_name VARCHAR(255) NOT NULL,
    last_name VARCHAR(255) NOT NULL,
    email VARCHAR(255) UNIQUE NOT NULL,
    password VARCHAR(255) NOT NULL,
    is_admin BOOLEAN DEFAULT FALSE,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

-- User preferences and profile data
CREATE TABLE IF NOT EXISTS user_profiles (
    id CHAR(36) PRIMARY KEY,
    user_id CHAR(36) UNIQUE NOT NULL,
    selected_character INTEGER DEFAULT 0,
    highest_level_reached INTEGER DEFAULT 1,
    total_playtime FLOAT DEFAULT 0.0,
    unlocked_worlds TEXT DEFAULT 'Space',  -- Comma-separated list
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE
);

-- Games table - tracks game sessions
CREATE TABLE IF NOT EXISTS games (
    id CHAR(36) PRIMARY KEY,
    player_name VARCHAR(100) NOT NULL,
    level INTEGER DEFAULT 1 NOT NULL,
    world VARCHAR(50) DEFAULT 'Space' NOT NULL,
    score INTEGER DEFAULT 0,
    status VARCHAR(20) DEFAULT 'active',  -- active, completed, game_over
    time_elapsed FLOAT DEFAULT 0.0,
    user_id CHAR(36) NOT NULL,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE
);

-- Scores table - stores high scores
CREATE TABLE IF NOT EXISTS scores (
    id CHAR(36) PRIMARY KEY,
    player_name VARCHAR(100) NOT NULL,
    value INTEGER NOT NULL,
    level INTEGER NOT NULL,
    user_id CHAR(36) NOT NULL,
    game_id CHAR(36),
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE,
    FOREIGN KEY (game_id) REFERENCES games(id) ON DELETE SET NULL
);

-- Level completion tracking
CREATE TABLE IF NOT EXISTS level_completions (
    id CHAR(36) PRIMARY KEY,
    user_id CHAR(36) NOT NULL,
    world VARCHAR(50) NOT NULL,
    level INTEGER NOT NULL,
    best_score INTEGER DEFAULT 0,
    best_time FLOAT DEFAULT 0.0,
    completed_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE,
    UNIQUE(user_id, world, level)
);

-- Indexes for performance
CREATE INDEX IF NOT EXISTS idx_users_email ON users(email);
CREATE INDEX IF NOT EXISTS idx_games_user_id ON games(user_id);
CREATE INDEX IF NOT EXISTS idx_games_status ON games(status);
CREATE INDEX IF NOT EXISTS idx_scores_user_id ON scores(user_id);
CREATE INDEX IF NOT EXISTS idx_scores_level ON scores(level);
CREATE INDEX IF NOT EXISTS idx_scores_value ON scores(value);
CREATE INDEX IF NOT EXISTS idx_level_completions_user_id ON level_completions(user_id);
CREATE INDEX IF NOT EXISTS idx_level_completions_world_level ON level_completions(world, level);
