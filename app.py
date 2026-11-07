"""
WatchNext - Streamlit frontend
Run with:
    py -m streamlit run app.py
"""

import os
import subprocess
import streamlit as st

EXE_NAMES = ["watchnext.exe", "watchnext"]
EXE_FOLDERS = ["", "build", "build/Debug", "build/Release"]
MOVIES_FILE = "data/movies.dat"
SUBPROCESS_TIMEOUT = 180
MAX_MATCHES = 50
ALGOS = {"BFS": "bfs", "Dijkstra": "dijkstra"}


@st.cache_data
def load_movies():
   #Read movies.dat and return {title: genres_string}
    movies = {}

    if not os.path.exists(MOVIES_FILE):
        return movies

    with open(MOVIES_FILE, encoding="latin-1") as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            parts = line.split("::")
            if len(parts) != 3:
                continue
            num, title, genres = parts
            genres = genres.replace("|", ", ")
            movies[title] = genres
    return movies


def find_exe():
    #checks both lists to find path
    for folder in EXE_FOLDERS:
        for name in EXE_NAMES:
            path = os.path.join(folder, name)
            if os.path.exists(path):
                return path
    return None


def run_backend(title, algo):
    #runs C++ code and reads what the program prints
    exe = find_exe()
    if exe is None:
        raise RuntimeError(
            "Could not find the compiled program."
        )
    try:
        result = subprocess.run([exe, title, algo], capture_output=True, text=True, encoding="latin-1", timeout=SUBPROCESS_TIMEOUT)
    except subprocess.TimeoutExpired:
        raise RuntimeError(f"The program did not finish within {SUBPROCESS_TIMEOUT} seconds.")

    if result.returncode != 0:
        stderr = (result.stderr or "").strip()
        raise RuntimeError(f"The program crashed (exit code {result.returncode}).\n\n{stderr}")
    return result.stdout

def parse_output(text):
    stats = None
    recs = []
    for line in text.splitlines():
        line = line.strip()
        if not line:
            continue
        if line.startswith("ERROR|"):
            message = line.split("|", 1)[1]
            return None, [], message or "backend reported an error."
        if line.startswith("STATS|"):
            parts = line.split("|")
            if len(parts) == 3:
                stats = {"runtime": parts[1], "nodes": parts[2]}
            continue
        parts = line.split("|")
        if len(parts) == 3:
            recs.append({"title": parts[0],"score": parts[1],"path": parts[2],})
    return stats, recs, None

#depends on algorithms
def score_label(algo):
    if algo == "bfs":
        return "Shared viewers"
    return "Distance"


def show_results(recs, stats, algo, movies, shared=None):
    """Draw one list of results. Compare mode calls this twice."""
    if stats:
        st.caption(f"{stats['runtime']} ms, {stats['nodes']} nodes visited")

    if not recs:
        st.info("No recommendations returned.")
        return

    for rank, rec in enumerate(recs, start=1):
        title = rec["title"]

        # star the titles that showed up in both lists
        star = " *" if shared and title in shared else ""
        st.markdown(f"**{rank}. {title}**{star}")

        genres = movies.get(title)
        if genres:
            st.caption(genres)

        st.write(f"{score_label(algo)}: {rec['score']}")
        st.caption(rec["path"])


def show_single(title, algo, movies):
    """Run one algorithm and draw it."""
    stats, recs, error = parse_output(run_backend(title, algo))

    if error:
        st.warning(error)
        return

    show_results(recs, stats, algo, movies)


def show_compare(title, movies):
    """Run both algorithms and draw them side by side."""
    bfs_stats, bfs_recs, bfs_error = parse_output(run_backend(title, "bfs"))
    dij_stats, dij_recs, dij_error = parse_output(run_backend(title, "dijkstra"))

    if bfs_error or dij_error:
        st.warning(bfs_error or dij_error)
        return

    # titles both algorithms agreed on
    shared = {r["title"] for r in bfs_recs} & {r["title"] for r in dij_recs}

    left, right = st.columns(2)
    with left:
        st.subheader("BFS")
        show_results(bfs_recs, bfs_stats, "bfs", movies, shared)
    with right:
        st.subheader("Dijkstra")
        show_results(dij_recs, dij_stats, "dijkstra", movies, shared)

    if shared:
        st.success(f"{len(shared)} movies appear in both lists (marked with *).")



st.title("WatchNext")
st.write("Movie recommendations from a graph of 1 million MovieLens ratings.")

movies = load_movies()
if not movies:
    st.error(f"Could not read {MOVIES_FILE}. The data folder has to sit next to app.py.")
    st.stop()

search = st.text_input("Search for a movie").strip()
if not search:
    st.info("Search for a movie to get started.")
    st.stop()

matches = [t for t in movies if search.lower() in t.lower()]
if not matches:
    st.warning("No movies match that search.")
    st.stop()

title = st.selectbox(f"Pick a movie ({len(matches)} matches)", matches[:MAX_MATCHES])

choice = st.radio("Algorithm", list(ALGOS) + ["Compare both"], horizontal=True)

if st.button("Recommend"):
    try:
        if choice == "Compare both":
            show_compare(title, movies)
        else:
            show_single(title, ALGOS[choice], movies)
    except RuntimeError as error:
        st.error(str(error))
