from app import create_app, db
from app.models.user import User
from app.models.place import Place
from app.models.review import Review
from app.models.amenity import Amenity
import sqlalchemy as sa
import uuid

app = create_app()

with app.app_context():
    print("🎯 TEST COMPLET DES 4 MODÈLES SQLALCHEMY")
    print("=" * 50)
    
    inspector = sa.inspect(db.engine)
    tables = inspector.get_table_names()
    print(f"📊 Tables dans la base : {tables}")
    
    print("\n🔍 Test Place - Vérification ID...")
    place_test = Place(
        title="Test Place",
        description="Test description",
        price=100.0,
        latitude=48.8566,
        longitude=2.3522,
        owner_id=str(uuid.uuid4())
    )
    print(f"Place ID après __init__: {hasattr(place_test, 'id')}")
    print(f"Place ID value: {getattr(place_test, 'id', 'NO ID')}")
    
    import time
    unique_email = f"test{int(time.time())}@example.com"
    
    print(f"\n👤 Création User: {unique_email}")
    user = User(
        first_name="TestUser", 
        last_name="ModelTest",
        email=unique_email,
        password="test123"
    )
    user.save()
    print(f"✅ User créé - ID: {user.id}")
    
    print(f"\n🏠 Création Place...")
    place = Place(
        title="Beautiful Apartment",
        description="Lovely place in the city",
        price=120.0,
        latitude=48.8566,
        longitude=2.3522,
        owner_id=user.id
    )
    print(f"Place avant save - ID: {getattr(place, 'id', 'NO ID')}")
    place.save()
    print(f"✅ Place créée - ID: {place.id}")
    
    print(f"\n⭐ Création Review...")
    review = Review(
        text="Amazing place! Highly recommend!",
        rating=5,
        place_id=place.id,
        user_id=user.id
    )
    review.save()
    print(f"✅ Review créée - ID: {review.id}")
    
    print(f"\n🏊 Création Amenity...")
    amenity = Amenity(name=f"Pool{int(time.time())}")
    amenity.save()
    print(f"✅ Amenity créée - ID: {amenity.id}")
    
    print(f"\n🔍 Vérification récupération...")
    saved_user = User.query.get(user.id)
    saved_place = Place.query.get(place.id)
    saved_review = Review.query.get(review.id)
    saved_amenity = Amenity.query.get(amenity.id)

    print(f"✅ User récupéré: {saved_user.email}")
    print(f"✅ Place récupérée: {saved_place.title}")
    print(f"✅ Review récupérée: {saved_review.text[:20]}...") 
    print(f"✅ Amenity récupérée: {saved_amenity.name}")

    print("\n🎉" + "="*47 + "🎉")
    print("🎯 TÂCHE 7 COMPLÈTEMENT RÉUSSIE !")
    print("🎯 Les 4 modèles sont PARFAITEMENT mappés à SQLAlchemy !")
    print("🎉" + "="*47 + "🎉")