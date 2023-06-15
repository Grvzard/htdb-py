
import htdb

def test():
    db = htdb.Htdb()

    for i in range(100):
        db.set(i, "zzzz")

    db.set(101, "xxxx")
    assert db.get(101) == b"xxxx"

    db.set("102", "yyyy")
    assert db.get("102") == b"yyyy"

    assert len(db) == 102
