db = [ 'Is your animal green?', ['frog'], ['squirrel'] ]


def getyesno():
    x = raw_input()
    while x == '' or (x[0] != 'y' and x[0] != 'n'):
	print 'Please type yes or no.'
	x = raw_input()
    return x[0] == 'y'


while True:

    print 'Think of an animal.'
    p = db
    while len(p) > 1:
	print p[0]
	if getyesno():
	    p = p[1]
	else:
	    p = p[2]
    print 'Is your animal a ' + p[0] + '?'
    if getyesno():
	print 'I got it!'
	print
    else:
	print 'Aw, I lose.  What animal were you thinking of?'
	newanimal = raw_input()
	print 'Tell me a question which would distinguish a ' + p[0] + ' from a ' + newanimal + '.'
	question = raw_input()
	print 'What would be the answer for a ' + newanimal + '?'
	if getyesno():
	    p += [[newanimal]]
	    p += [[p[0]]]
	else:
	    p += [[p[0]]]
	    p += [[newanimal]]
	p[0] = question
	print 'I\'ll remember that.'
	print
