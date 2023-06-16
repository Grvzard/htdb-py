
import htdb


def test_1():
    db = htdb.Htdb()

    for i in range(100):
        db.set(i, "zzzz")

    assert ("101" in db) == False
    db.set("101", "yyyy")
    assert db.get("101") == b"yyyy"
    assert ("101" in db) == True

    db.remove("101")

    try:
        db.get("101")
    except KeyError:
        ...
    except Exception as e:
        raise e
    else:
        assert False

    try:
        db.get(101)
    except KeyError:
        ...
    else:
        assert False

    assert len(db) == 100


def test_2():
    print()

    db = htdb.Htdb()

    for _ in range(2):
        for i in range(10000):
            db.set(i, "zzzz")
        for i in range(10000):
            db.remove(i)
