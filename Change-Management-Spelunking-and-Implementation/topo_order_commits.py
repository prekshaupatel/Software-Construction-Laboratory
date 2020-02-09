# Keep the function signature,
# but replace its body with your implementation
import os
import sys
import zlib

class CommitNode:
    def __init__(self, commit_hash):
        """                                                                     
        :type commit_hash: str                                                  
        """
        self.commit_hash = commit_hash
        self.parents = set()
        self.children = set()
        self.visited = 0
        self.branch_names = []






def topo_order_commits():
    
    def find_top():
        path=os.getcwd()
        while path != '/':
            for d in next(os.walk(path))[1]:
                if d == '.git':
                    return path
            path = os.path.dirname(path)
        return path
    #to find the directory with .git                                           
    def hash_parent(b_hash):
        obj_path = top + '/.git/objects/' + b_hash[:2] +'/' + b_hash[2:40]
        compressed = open(obj_path, 'rb').read()
        decompressed = zlib.decompress(compressed)
        decompressed = decompressed.decode()
        list_elements = decompressed.split()
        i = 0
        c = 0
        max_length = len(list_elements)-1
        while (i < max_length):
            if (list_elements[i] == 'parent'):
                i = i+1
                c =c+1
                hash_map[b_hash].parents.add(list_elements[i])
                if list_elements[i] in hash_map:
                    hash_map[list_elements[i]].children.add(b_hash)
                else:
                    list_objs.append(list_elements[i])
                    hash_map[list_elements[i]] = CommitNode(list_elements[i])
                    hash_map[list_elements[i]].children.add(b_hash)
            i = i+1
        if c == 0:
            root_commits.add(b_hash)


    def list_arrange():
        j = 0
        temp_val = temp_stack[len(temp_stack) - 1]
        for child in sorted(hash_map[temp_val].children):
            if hash_map[child].visited == 0:
    #            hash_map[child].visted = 1                                         
                temp_stack.append(child)
                j = j+1
        if j == 0:
            temp_stack.pop()
            if temp_val not in topo_stack:
                topo_stack.append(temp_val)
                hash_map[temp_val].visited = 1

    #######################################################                         
    #########         main code                ############                         
    #######################################################

    top = find_top()
    if top == '/':
        sys.stderr.write('Not inside a Git repository\n')
        sys.exit(1)
    list_objs = []
    root_commits = set()
    hash_map = dict()
    headlist = top + '/.git/refs/heads'
    for heads in sorted(os.listdir(headlist)):
        filepath = headlist + '/' + heads
        file = open(filepath,"r")
        branch_hash = file.readline()[:-1]
        if branch_hash not in hash_map:
            hash_map[branch_hash] = CommitNode(branch_hash)
        hash_map[branch_hash].branch_names.append(heads)
        hash_parent(branch_hash)
        file.close()
    while len(list_objs) != 0:
        branch_hash = list_objs.pop()
        hash_parent(branch_hash)

    temp_stack = []
    topo_stack = []
    for node in sorted(root_commits):
    #    hash_map[node].visited = 1                                                 
        temp_stack.append(node)
        while (len(temp_stack) != 0):
            list_arrange()

    k = 0
    if len(topo_stack) != 0:
        curr = topo_stack[k]
        add = ''
        for a in sorted(hash_map[curr].branch_names):
            add = add +' ' + a
        print(curr+add)
    k = k+1
    len_stack = len(topo_stack)
    while k < len_stack:
        prev = curr
        curr = topo_stack[k]
        if curr not in hash_map[prev].parents:
            t = ''
            for par in sorted(hash_map[prev].parents):
                t = t + ' ' + par
            print(t[1:]+'=\n')
            t = ''
            for child in sorted(hash_map[curr].children):
                t = t + ' ' + child
            print('='+t[1:])
        add = ''
        for a in sorted(hash_map[curr].branch_names):
            add = add + ' ' + a
        print(curr+add)
        k = k+1


        
if __name__ == '__main__':
    topo_order_commits()
