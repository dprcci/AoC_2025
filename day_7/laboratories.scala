import scala.collection.mutable.HashMap
import scala.collection.immutable.Set

object Laboratories {
  def read_input(
      file_name: String
  ): (Int, HashMap[Int, Set[Int]], (Int, Int)) = {
    val source = scala.io.Source.fromFile(file_name)
    val lines: List[String] =
      try source.getLines().toList
      finally source.close()

    var splittersMap = new HashMap[Int, Set[Int]]()
    lines
      .drop(1)
      .zipWithIndex
      .foreach({
        case (l, l_idx) => {
          val splitters = l.zipWithIndex.collect {
            case (c, col_idx) if c == '^' => col_idx
          }.toSet
          splittersMap.update(l_idx, splitters)
        }
      })
    return (
      lines.head.indexOf('S'),
      splittersMap,
      (lines.head.size, splittersMap.size)
    )
  }

  def fire_tychon_beam(
      startPosition: Int,
      splittersMap: HashMap[Int, Set[Int]],
      dimensions: (Int, Int)
  ): Int = {
    splittersMap.values.foldLeft((0, Set(startPosition))) {
      case ((nbSplits, beams), splitters) => {
        if (splitters.isEmpty) {
          (nbSplits, beams)
        } else {
          val (toSplit, straight) = beams.partition(splitters.contains)
          val split = toSplit
            .flatMap(idx =>
              Set(idx - 1, idx + 1).filter(i => i >= 0 && i < dimensions(0))
            )
            .toSet
          (nbSplits + toSplit.size, straight ++ split)
        }
      }
    }(0)
  }

  def fire_quantum_tychon_beam(startPosition: Int, splittersMap: HashMap[Int, Set[Int]], dimensions: (Int, Int)) : Long = {
    //dimensions = col, row
    var memoized = new HashMap[(Int, Int), Long]()
    def findNextSplitDepth(startPosition: (Int, Int)): Option[Int] = {
        splittersMap.filter((row, splitters) => row > startPosition(1) && splitters.contains(startPosition(0))).keys.minOption
    }

    def explore(startPosition: (Int, Int)): Long = {
        if (memoized.contains(startPosition)) {
            return memoized(startPosition)
        }
        val nextSplitRowIdx = findNextSplitDepth(startPosition)
        if (!nextSplitRowIdx.isDefined || nextSplitRowIdx.get == dimensions(1)){
            (startPosition(1) until dimensions(1)).foreach(rowIdx => memoized.update((startPosition(0), rowIdx), 1))
            return 1
        }
        val col = startPosition(0)
        val row = nextSplitRowIdx.get
        var totalPossibleSubSplits: Long = 0
        if (col - 1 >= 0) {
            totalPossibleSubSplits += explore((col - 1, row + 1))
        }
        if (col + 1 < dimensions(0)){
            totalPossibleSubSplits += explore((col + 1, row + 1))
        }
        memoized.update(startPosition, totalPossibleSubSplits)
        totalPossibleSubSplits
    }
    explore((startPosition, 0))
  }
}

@main
def main(inputFile: String = "example.txt"): Unit = {
  val (startingPosition, splittersMap, dimensions) =
    Laboratories.read_input(inputFile)
  val nbBeamSplits =
    Laboratories.fire_tychon_beam(startingPosition, splittersMap, dimensions)
  println("Number of beam splits " + nbBeamSplits)
  val nbQuantumBeamSplits = Laboratories.fire_quantum_tychon_beam(startingPosition, splittersMap, dimensions)
  println("Number of quantum timelines " + nbQuantumBeamSplits)
}
