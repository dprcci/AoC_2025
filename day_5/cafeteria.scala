import scala.math._

case class Interval(low: Long, high: Long)
case class Node(
  interval: Interval,
  max: Long, 
  left: Option[Node],
  right: Option[Node]
)

object IntervalTree {
    def insert(tree: Option[Node], interval: Interval): Node = {
        tree match {
            case None => Node(interval, interval.high, None, None)
            
            case Some(Node(currentInterval, currentMax, left, right)) => {
                val newMax = max(currentMax, interval.high)
                //overlap
                if (interval.low < currentInterval.low) {
                    val newLeft = insert(left, interval)
                    Node(currentInterval, newMax, Some(newLeft), right)
                } else {
                    val newRight = insert(right, interval)
                    Node(currentInterval, newMax, left, Some(newRight))
                }
            }
        }
    }

    def contains(tree: Option[Node], searched: Long): Boolean =  {
        tree match {
            case None => false
            case Some(Node(currentInterval, currentMax, left, right)) => {
                if (searched > currentMax) {
                    false 
                } 
                else if (currentInterval.low <= searched && searched <= currentInterval.high) {
                    true
                } else {
                    val is_left = if (searched <= left.map(_.max).getOrElse(Long.MinValue)) then contains(left, searched) else false
                    val is_right = if (searched <= right.map(_.max).getOrElse(Long.MinValue)) then contains(right, searched) else false
                    is_left || is_right
                }
            }  
        }
    }

    def traverse(tree: Option[Node]): List[Interval] = {
        tree match {
            case None => Nil
            case Some(Node(currentInterval, currentMax, left, right)) =>
            traverse(left) ::: List(currentInterval) ::: traverse(right)
        }
    }
}

def read_input(file_name: String): (List[Interval], List[Long]) = {
    val source = scala.io.Source.fromFile(file_name)
    val lines: List[String] = try source.getLines().toList finally source.close()
    val splitIndex = lines.indexWhere(_.trim.isEmpty)

    if (splitIndex < 0){
        throw new Exception("file coudln't be parsed")
    }
    val raw_intervals = lines.take(splitIndex)
    val raw_ingredients = lines.drop(splitIndex + 1) 
    (raw_intervals.map(s => s.split('-')).map(range => Interval(range(0).toLong, range(1).toLong)), raw_ingredients.map(_.toLong))
}


def find_fresh_ingredients(tree: Option[Node], ingredients: List[Long]): Int = {
    ingredients.map(IntervalTree.contains(tree, _)).filter(identity).size
}

def find_all_possibly_fresh(tree: Option[Node]) : Long = {
    IntervalTree.traverse(tree).sortBy(_.low).foldLeft(List.empty[Interval]){
        case (Nil, interval) => List(interval)
        case (head :: tail, interval) if interval.low <= head.high =>
            Interval(head.low, max(head.high, interval.high)) :: tail
        case (acc, interval) => interval :: acc
    }.map(interval => (interval.high - interval.low) + 1).sum
}

@main
def cafeteria(inputFile: String = "example.txt"): Unit = {
    val (intervals, ingredients) = read_input(inputFile)
    val tree = intervals.foldLeft(Option.empty[Node]) {(acc, interval) => Some(IntervalTree.insert(acc, interval))}
    val nbFreshIngredients = find_fresh_ingredients(tree, ingredients)
    printf("Number of fresh ingredients %d\n", nbFreshIngredients)
    val nbPossiblyFreshIngredients = find_all_possibly_fresh(tree)
    printf("Number of possible fresh ingredients %d\n", nbPossiblyFreshIngredients)
}