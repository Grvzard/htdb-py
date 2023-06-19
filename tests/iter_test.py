
import htdb

def test():
    print()

    db = htdb.Htdb()

    for i in range(5):
        db.set(i, i)
    
    assert db.list() == list((i, i) for i in range(5))

    for i in range(5):
        db.remove(i)

    assert db.list() == []

    for i in range(5):
        db.set(str(i), str(i))
    
    assert db.list() == list(
        (bytes(str(i), 'utf-8'), bytes(str(i), 'utf-8'))
        for i in range(5)
    )
