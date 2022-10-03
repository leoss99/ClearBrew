import json
import pymongo
import sys
import boto3
import os
import ast
import bson
import datetime
from bson.objectid import ObjectId
from bson.timestamp import Timestamp

db_client = None
doc_uri = "mongodb://clearbrew:bestcoffeeever@docdb-2022-03-17-16-35-59.cluster-cglum3i3eqqn.us-east-1.docdb.amazonaws.com:27017/?ssl=true&ssl_ca_certs=rds-combined-ca-bundle.pem&replicaSet=rs0&readPreference=secondaryPreferred&retryWrites=false"
db_name = "clearbrew"
datetime_str = "%Y-%m-%dT%H:%M:%S"

brewers = ["Chemex", "V60", "AeroPress", "Moka Pot", "French Press", "Espresso"]
origins = ["Columbia", "Brazil", "Honduras", "Ecuador"]
species = ["Arabica", "Robusta", "Liberica", "Excelsa"]
roasts = ["Light", "Medium", "Dark", "Extra Dark"] 

def stringify(doc):
    if (type(doc) == str):
        return doc
    if (type(doc) == dict):
        for k, v in doc.items():
            doc[k] = stringify(v)
    if (type(doc) == list):
        for i in range(len(doc)):
            doc[i] = stringify(doc[i])
    if (type(doc) == bson.ObjectId):
        doc = str(doc)
    if (type(doc) == datetime.datetime):
        doc = doc.strftime(datetime_str)
    if (type(doc) == Timestamp):
        doc = doc.as_datetime().strftime(datetime_str)
    return doc

## DOCUMENTDB CONNECTION
def get_db_client():
    """Return an authenticated connection to DocumentDB"""
    # Use a global variable so Lambda can reuse the persisted client on future invocations
    global db_client
    if db_client is None:
        try:
            db_client = pymongo.MongoClient(doc_uri)
        except Exception as ex:
            raise
    return db_client

def collection_from_event(collection_name):
    get_db_client()
    db = db_client[db_name]
    collection = db[collection_name]
    return collection

def existing_record(collection_name, record_filter):
    collection = collection_from_event(collection_name)    
    res = collection.find_one(filter=record_filter)
    if res != None:
        return True
    return False


## USER METHOD
def handle_get_user(event):
    collection = collection_from_event("users")
    input_qs = event["queryStringParameters"]
    # If the request did not include a recipe_id
    try:
        user_id = input_qs["_id"]
    except:
        return {"error_message" : "There was no '_id' field passed."},  400
    filter = {"_id": ObjectId(user_id)}
    res = collection.find_one(filter=filter)
    # If no record is found, then return 404
    if res == None:
        return {"error_message" : "No record was found matching the '_id' field."}, 404
    # If a record is found, then return it, and 200
    return stringify(res), 200

def handle_post_user(event):
    collection = collection_from_event("users")
    #convert json to python dictionary
    body = event["body"]
    #checking the object that was passed
    try:
        keys = body.keys()
        assert "username" in keys
        assert "email" in keys
        assert "password" in keys
        if existing_record("users", {"username": body["username"]}):
            return {"error_message" : "A user already exists with the given username."}, 400
    except Exception as e:
        return {"error_message" : "The user record did not have the required fields.", "error" : str(e), "username" : body["username"], "email" : body["email"], "password" : body["password"]}, 400

    body["recipesCompleted"] = []
    body["recipesAdded"] = []
    body["numRecipesCompleted"] = 0
    post_id = collection.insert_one(body)
    body["_id"] = post_id.inserted_id
    return stringify(body), 201
  
def handle_patch_user(event):
    collection = collection_from_event("users")
    options = ["email", "username", "password"]
    filter = None
    update = None
    #convert json to python dictionary
    body = json.loads(event["body"])
    try:
        user_id = body["_id"]
        if not existing_record("users", {"_id": user_id}):
            return {"error_message" : "No record with a matching '_id' was found."}, 404 

        update = body["update"]
        # If the request is to update fields that are not valid
        for key in update.keys():
            if key not in options:
                return None, 400
            if existing_record("users", {"username" : update["username"]}):
                return {"Username already exists."}, 400
    except:
        return {"error_message" : "Invalid request."}, 400
    filter = {"_id": ObjectId(user_id)}
    res = collection.update_one(filter, update)
    return stringify(res.raw_result), 200

## RECIPE METHODS

def handle_get_one_recipe(event):
    collection = collection_from_event("recipes")
    input_qs = event["queryStringParameters"]
    # If the request did not include a recipe_id
    try:
        recipe_id = input_qs["_id"]
    except:
        return {"error_message" : "There was no '_id' field passed."},  400
    
    filter = {"_id": ObjectId(recipe_id)}
    res = collection.find_one(filter=filter)
    # If no record is found, then return 404
    if res == None:
        return {"error_message" : "No record was found matching the '_id' field."}, 404
  # If a record is found, then return it, and 200
    return stringify(res), 200


def handle_post_one_recipe(event):
    collection = collection_from_event("recipes")
    body = event["body"]
    #checking the object that was passed
    try:
        keys = body.keys()
        assert "name" in keys
        assert "steps" in keys
        assert "estimatedTime" in keys
        assert "brewer" in keys
        assert "origin" in keys
        assert "species" in keys
        assert "roast" in keys
        assert existing_record("users", {"_id": ObjectId(body["user"])})
        body["rating"] = 0
        body["timesMade"] = 0
        body["numSteps"] = len(body["steps"])
    except:
        return {"error_message" : "The recipe record did not have the required fields."}, 400
    
    post_id = collection.insert_one(body)
    return {"new_recipe_id" : str(post_id.inserted_id)}, 200 
    
        
def handle_patch_one_recipe(event):
    collection = collection_from_event("recipes")
    options = ["estimatedTime", "name", "brewer", "origins", "species", "roast"]
    filter = None
    update = None
    #convert json to python dictionary
    body = json.loads(event["body"])
    try:
        recipe_id = body["_id"]
        if not existing_record("recipes", {"_id": body["_id"]}):
            return {"error_message" : "No record with a matching '_id' was found."}, 404 

        update = body["update"]
        # If the request is to update fields that are not valid
        for key in update.keys():
            if key not in options:
                return {"error_message" : "Invalid update request."}, 400
    except:
        return {"error_message" : "Invalid request."}, 400
    filter = {"_id": ObjectId(recipe_id)}
    res = collection.update_one(filter, update)
    return stringify(res.raw_result), 200

def handle_get_all_recipe(event):
    collection = collection_from_event("recipes")
    res = collection.find().limit(3)
    result = []
    for doc in res:
        result.append(doc)
    return stringify(result), 200

# Need to implement recommender
def handle_get_recommend_recipe(event):
    collection = collection_from_event("recipes")

    pipeline = [{ "$sample": { "size": 1 } }]

    res = collection.aggregate(pipeline)
    result = []
    for doc in res:
        result.append(doc)
    return stringify(result), 200

def handle_get_recipe_by_user(event):
    collection = collection_from_event("recipes")
    input_qs = event["queryStringParameters"]
    # If the request did not include a recipe_id
    try:
        user_id = input_qs["user"]
        if not existing_record("users", {"_id" : ObjectId(user_id)}):
            return {"error_message" : "No user '_id' matches."}, 400
    except:
        return {"error_message" : "There was no 'user' field passed."},  400
    
    filter = {"user": user_id}
    res = collection.find(filter=filter)
    # If no record is found, then return 404
    if res == None:
        return {"error_message" : "No records were found matching the user '_id' field."}, 404
    # If any record is found, then return it, and 200
    result = []
    for doc in res:
        result.append(doc)
    return stringify(result), 200
    
def isint(num):
    try:
        int(num)
        return True
    except ValueError:
        return False

def handle_post_completed_recipe(event):
    collection = collection_from_event("completedRecipes")
    body = event["body"]
    #checking the object that was passed
    try:
        user_id = body["user_id"]
        if not existing_record("users", {"_id" : ObjectId(user_id)}):
            return {"error_message" : "The user '_id' is not valid"}, 400
        recipe_id = body["recipe_id"]
        if not existing_record("recipes", {"_id" : ObjectId(recipe_id)}):
            return {"error_message" : "The recipe '_id' is not valid"}, 400
        if not isint(body["rating"]):
            return {"error_message" : "Invalid 'rating' data type"}, 400
    except:
        return {"error_message": "Invalid/missing request fields."}, 400

    collection.insert_one(body)
    #todo
    # update the user to reflect the completed recipe

    # update the recipe rating
    return stringify(body), 201
        
# Lambda Handler
def lambda_handler(event, context):

    httpMethod = event["httpMethod"]
    retval = "Received"
    path = event["path"]
    error_code = 0

    if httpMethod == "GET":
        if path == "/recipe":
            retval, error_code = handle_get_one_recipe(event)
        elif path == "/recipe/all":
            retval, error_code = handle_get_all_recipe(event)
        elif path == "/recipe/user":
            retval, error_code = handle_get_recipe_by_user(event)
        elif path == "/recipe/recommend":
            retval, error_code = handle_get_recommend_recipe(event)
        elif path == "/user":
            retval, error_code = handle_get_user(event)

    if httpMethod == "POST":
        if path == "/recipe":
            retval, error_code = handle_post_one_recipe(event)
        elif path == "/user":
            retval, error_code = handle_post_user(event)
        elif path == "/completedRecipe":
            retval, error_code = handle_post_completed_recipe(event)
    
    if httpMethod == "PATCH":
        if path == "/recipe":
            retval, error_code = handle_patch_one_recipe(event)
        elif path == "/user":
            retval, error_code = handle_patch_user(event)
                
    return {
        'headers': {
            "Access-Control-Allow-Headers" : "Content-Type",
            "Access-Control-Allow-Origin": "*",
            "Access-Control-Allow-Methods": "OPTIONS,POST,GET,PUT,PATCH"
        },
        'statusCode': error_code,
        'body': json.dumps(retval)
    }