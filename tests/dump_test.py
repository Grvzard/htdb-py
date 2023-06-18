
import htdb


def test(tmp_path):
    print()

    db = htdb.Htdb()

    for i in range(5):
        db.set(i, "zzzz")

    for i in range(5):
        db.set(f"{i}", "yyyy")

    db.dump(tmp_path / "dump.htdb")
    print(len(db))
    db.dump()

    db2 = htdb.Htdb()
    db2.load(tmp_path / "dump.htdb")
    print(len(db2))
    db2.dump()

    for i in range(5):
        assert db2.get(i) == b"zzzz"
        assert db2.get(f"{i}") == b"yyyy"
