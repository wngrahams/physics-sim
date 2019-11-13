with open('plane.obj', 'w') as f1:
    f1.write("g Plane\n")

    # Verticies
    for x in range(-5, 6):
        for y in range(-5, 6):
            xf = x + 0.000000
            yf = y + 0.000000
            f1.write("v %f %f 0.000000\n" % (xf, yf))

    # Texture coordinates
    f1.write("vt 0.000000, 0.000000\n")
    f1.write("vt 0.000000, 1.000000\n")
    f1.write("vt 1.000000, 0.000000\n")
    f1.write("vt 1.000000, 1.000000\n")

    # Vertex normals
    f1.write("vn 0.000000, 0.000000, 1.000000\n")

    f1.write("s 1\n")

    # faces (CCW):
    for col in range(0, 10):
        for row in range(0, 10):
            # 2 triangles for each square:
            v1 = row + col*11 + 1
            v2 = (row+1) + ((col+1)*11) + 1
            v3 = (row+1) + (col*11) + 1
            f1.write("f %d/2/1 %d/3/1 %d/1/1\n" % (v1, v2, v3))
            v1 = row + col*11 + 1
            v2 = row + ((col+1)*11) + 1
            v3 = (row+1) + ((col+1)*11) + 1
            f1.write("f %d/2/1 %d/4/1 %d/3/1\n" % (v1, v2, v3))


f1.close()
