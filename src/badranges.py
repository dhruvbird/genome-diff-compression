def getline():
    try:
        l = raw_input()
        return l
    except:
        return None


buff = [ ]
nignored = 0
l = getline()

good_read_len = 100
max_bad_reads_allowed = 200

while l != None:
    val = int(l.split()[3])
    if val < good_read_len:
        buff.append(l)
    else:
        if len(buff) < max_bad_reads_allowed:
            # Print it.
            for i in buff:
                print i
        else:
            # If it's at least than max_bad_reads_allowed,
            # then we skip the whole buffer
            nignored += len(buff)

        buff = [ ]
        print l

    l = getline()

# print "Numbers ignored: %d" % nignored
