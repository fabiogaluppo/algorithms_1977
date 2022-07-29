//Sample by Fabio Galuppo - http://member.acm.org/~fabiogaluppo - fabiogaluppo@acm.org
//July 2022
//Inspired by Don Knuth's article on Algorithms for Scientific American in 1977
//This file is licensed to you under the MIT license

//dotnet fsi summing_up.fsx .\data\word_count.txt

open System;;
open System.IO;;
open System.Collections.Generic;;

let a = File.ReadLines(fsi.CommandLineArgs.[1]) |> Seq.map(fun x -> x.Split([| ' ' |], StringSplitOptions.RemoveEmptyEntries)) |> Seq.map(fun x -> x.[0]);;
printfn "%A\n" a;;

a |> Seq.findIndex(fun x -> x.Equals("FROM"));;
printfn "%A\n" it;;

let b = a |> Seq.toArray |> Array.sortWith(fun x y -> String.Compare(x, y));;
printfn "%A\n" b;;

Array.BinarySearch(b, "FROM");;
printfn "%A\n" it;;

type BST_NODE = { data : String; left : BST_NODE option;  right : BST_NODE option };;

let rec bst_insert_recursive (node : BST_NODE option, data : String) =
  match node with
  | Some(x) -> Some(if (data < x.data) then { data = x.data; left = bst_insert_recursive(x.left, data); right = x.right }
                    else { data = x.data; left = x.left; right = bst_insert_recursive(x.right, data) })
  | None -> Some({ data = data; left = None; right = None });;

let root = bst_insert_recursive(None, "HELLO");;
printfn "%A\n" root;;

let root = bst_insert_recursive(root, "WORLD");;
printfn "%A\n" root;;

Seq.fold (fun root x -> bst_insert_recursive(root, x)) None a;;
printfn "%A\n" it;;

let ht = new HashSet<String>(a);;
printfn "%A\n" ht;;

ht.Contains("FROM");;
printfn "%A\n" it;;

ht.Contains("HELLO");;
printfn "%A\n" it;;