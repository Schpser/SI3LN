from app.models.user import User
from app.models.game import Game
from app.models.scores import Score
from app.models.user_profile import UserProfile
from app.models.level_completion import LevelCompletion
from app.persistence.repository import SQLAlchemyRepository

class HBnBFacade:
    def __init__(self):
        self.user_repo = SQLAlchemyRepository(User)
        self.game_repo = SQLAlchemyRepository(Game)
        self.score_repo = SQLAlchemyRepository(Score)
        self.user_profile_repo = SQLAlchemyRepository(UserProfile)
        self.level_completion_repo = SQLAlchemyRepository(LevelCompletion)

    def create_user(self, user_data):
        user_data_copy = user_data.copy()
        password = user_data_copy.pop('password', None)
        
        user = User(
            first_name=user_data_copy['first_name'],
            last_name=user_data_copy['last_name'],
            email=user_data_copy['email'],
            is_admin=user_data_copy.get('is_admin', False)
        )

        if password:
            user.hash_password(password)

        return self.user_repo.add(user)

    def get_user(self, user_id):
        return self.user_repo.get(user_id)

    def get_all_users(self):
        return self.user_repo.get_all()

    def get_user_by_email(self, email):
        return self.user_repo.get_by_attribute('email', email)

    def update_user(self, user_id, user_data):
        return self.user_repo.update(user_id, user_data)

    def delete_user(self, user_id):
        return self.user_repo.delete(user_id)

    # Game methods
    def create_game(self, game_data):
        game = Game(**game_data)
        return self.game_repo.add(game)

    def get_game(self, game_id):
        return self.game_repo.get(game_id)

    def get_all_games(self):
        return self.game_repo.get_all()

    def get_games_by_user(self, user_id):
        return self.game_repo.get_by_attribute('user_id', user_id)

    def get_active_game(self, user_id):
        """Get the user's active game"""
        from app import db
        games = db.session.query(Game).filter_by(
            user_id=user_id, 
            status='active'
        ).order_by(Game.created_at.desc()).first()
        return games

    def update_game(self, game_id, game_data):
        return self.game_repo.update(game_id, game_data)

    def delete_game(self, game_id):
        return self.game_repo.delete(game_id)

    # Score methods
    def create_score(self, score_data):
        score = Score(**score_data)
        return self.score_repo.add(score)

    def get_score(self, score_id):
        return self.score_repo.get(score_id)

    def get_all_scores(self):
        return self.score_repo.get_all()

    def get_scores_by_user(self, user_id):
        return self.score_repo.get_by_attribute('user_id', user_id)

    def get_scores_by_game(self, game_id):
        return self.score_repo.get_by_attribute('game_id', game_id)

    def get_top_scores(self, limit=10, level=None):
        """Get top scores, optionally filtered by level"""
        from app import db
        query = db.session.query(Score)
        
        if level:
            query = query.filter_by(level=level)
        
        return query.order_by(Score.value.desc()).limit(limit).all()

    def delete_score(self, score_id):
        return self.score_repo.delete(score_id)

    def create_place(self, place_data):
        place_data_copy = place_data.copy()

        amenity_ids = place_data_copy.pop('amenities', [])

        place = Place(**place_data_copy)

        for amenity_id in amenity_ids:
            amenity = self.amenity_repo.get(amenity_id)
            if amenity:
                place.amenities.append(amenity)
    
        return self.place_repo.add(place)

    def get_place(self, place_id):
        return self.place_repo.get(place_id)

    def get_all_places(self):
        return self.place_repo.get_all()

    def update_place(self, place_id, place_data):
        place_data_copy = place_data.copy()
        amenity_ids = place_data_copy.pop('amenities', None)

        place = self.place_repo.update(place_id, place_data_copy)

        if place and amenity_ids is not None:
            place.amenities = []

            for amenity_id in amenity_ids:
                amenity = self.amenity_repo.get(amenity_id)
                if amenity:
                    place.amenities.append(amenity)

            from app import db
            db.session.commit()

        return place

    def delete_place(self, place_id):
        return self.place_repo.delete(place_id)

    def get_places_by_owner(self, owner_id):
        return self.place_repo.get_by_attribute('owner_id', owner_id)

    def create_review(self, review_data):
        review = Review(**review_data)
        return self.review_repo.add(review)

    def get_review(self, review_id):
        return self.review_repo.get(review_id)

    def get_all_reviews(self):
        return self.review_repo.get_all()

    def get_reviews_by_place(self, place_id):
        return self.review_repo.get_by_attribute('place_id', place_id)

    def get_reviews_by_user(self, user_id):
        return self.review_repo.get_by_attribute('user_id', user_id)

    def create_amenity(self, amenity_data):
        amenity = Amenity(**amenity_data)
        return self.amenity_repo.add(amenity)

    def get_amenity(self, amenity_id):
        return self.amenity_repo.get(amenity_id)

    def get_all_amenities(self):
        return self.amenity_repo.get_all()

    def get_amenity_by_name(self, name):
        return self.amenity_repo.get_by_attribute('name', name)

    # UserProfile methods
    def create_user_profile(self, profile_data):
        profile = UserProfile(**profile_data)
        return self.user_profile_repo.add(profile)

    def get_user_profile(self, profile_id):
        return self.user_profile_repo.get(profile_id)

    def get_user_profile_by_user(self, user_id):
        return self.user_profile_repo.get_by_attribute('user_id', user_id)

    def update_user_profile(self, profile_id, profile_data):
        return self.user_profile_repo.update(profile_id, profile_data)

    def delete_user_profile(self, profile_id):
        return self.user_profile_repo.delete(profile_id)

    # LevelCompletion methods
    def create_level_completion(self, completion_data):
        from datetime import datetime
        if 'completed_at' not in completion_data:
            completion_data['completed_at'] = datetime.utcnow()
        completion = LevelCompletion(**completion_data)
        return self.level_completion_repo.add(completion)

    def get_level_completion(self, completion_id):
        return self.level_completion_repo.get(completion_id)

    def get_user_level_completion(self, user_id, world, level):
        """Get a specific level completion for a user"""
        from app import db
        return db.session.query(LevelCompletion).filter_by(
            user_id=user_id,
            world=world,
            level=level
        ).first()

    def get_user_completions(self, user_id, world=None):
        """Get all level completions for a user, optionally filtered by world"""
        from app import db
        query = db.session.query(LevelCompletion).filter_by(user_id=user_id)
        if world:
            query = query.filter_by(world=world)
        return query.all()

    def update_level_completion(self, completion_id, completion_data):
        from datetime import datetime
        completion_data['updated_at'] = datetime.utcnow()
        return self.level_completion_repo.update(completion_id, completion_data)

    def record_level_completion(self, user_id, world, level, score, time_elapsed):
        """Record or update a level completion with best score/time"""
        from datetime import datetime
        existing = self.get_user_level_completion(user_id, world, level)
        
        if existing:
            # Update if new score is better
            update_data = {}
            if score > existing.best_score:
                update_data['best_score'] = score
            if time_elapsed < existing.best_time or existing.best_time == 0:
                update_data['best_time'] = time_elapsed
            
            if update_data:
                update_data['updated_at'] = datetime.utcnow()
                return self.update_level_completion(existing.id, update_data)
            return existing
        else:
            # Create new completion
            return self.create_level_completion({
                'user_id': user_id,
                'world': world,
                'level': level,
                'best_score': score,
                'best_time': time_elapsed,
                'completed_at': datetime.utcnow()
            })

facade = HBnBFacade()