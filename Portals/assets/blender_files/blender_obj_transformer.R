

library(data.table)

################################
##### function definitions #####
################################


create_table <- function(strings, separator)
{
	separated <- unlist(strsplit(strings[1], split = separator))
	result <- data.frame(matrix(0, nrow = length(strings), ncol = length(separated)))
	
	for(i in 1:length(strings))
	{
		separated <- unlist(strsplit(strings[i], split = separator))
		for(j in 1:length(separated))
		{
			result[[j]][i] <- as.numeric(separated[j])
		}
	}
	
	return(result)
}

# vector_string <- vertex_lines[1]
get_vector <- function(vector_string, separator = " ")
{
	separated <- unlist(strsplit(x = vector_string, split = separator))
	separated <- separated[2:length(separated)]
	
	return(as.numeric(separated))
	# return(paste(separated, collapse = "\t"))
}


print_vertex_data <- function(file_content)
{
	
	file_content <- file_content[-grep(pattern = "# ", x = file_content)]
	
	vertex_lines <- file_content[grep(pattern = "v ", x = file_content)]
	texcoord_lines <- file_content[grep(pattern = "vt ", x = file_content)]
	normal_lines <- file_content[grep(pattern = "vn ", x = file_content)]
	faces_lines <- file_content[grep(pattern = "f ", x = file_content)]
	
	total_vertex_count <- 4*length(faces_lines)
	total_index_count <- 6*length(faces_lines)
	
	for(face in 1:length(faces_lines))
	{
		current_face <- unlist(strsplit(x = faces_lines[face], split = " "))
		current_face <- current_face[2:length(current_face)]
		
		face_table <- create_table(current_face, "/")
		
		for(i in 1:nrow(face_table))
		{
			cat(get_vector(vertex_lines[face_table[[1]]][i]))
			cat("\t")
			cat(get_vector(normal_lines[face_table[[3]]][i]))
			cat("\t")
			cat(get_vector(texcoord_lines[face_table[[2]]][i]))
			cat("\n")
		}
	}
}

count_triangles <- function(faces_lines, separator = " ")
{
	triangle_count <- 0
	for(i in 1:length(faces_lines))
	{
		tmp <- unlist(strsplit(x = faces_lines[i], split = separator))
		triangle_count <- triangle_count + length(tmp)-3
	}
	return(triangle_count)
}

count_vertices <- function(faces_lines, separator = " ")
{
	vertex_count <- 0
	for(i in 1:length(faces_lines))
	{
		tmp <- unlist(strsplit(x = faces_lines[i], split = separator))
		vertex_count <- vertex_count + length(tmp)-1
	}
	return(vertex_count)
}


###############################
##### main transformation #####
###############################

# set input and output files
# input_file <- "D:/cpp_codes/26_portals/Portals/assets/blender_files/export_with_comments.obj"
# output_file <- "D:/cpp_codes/26_portals/Portals/assets/blender_files/transformed.txt"

# input_file <- "D:/cpp_codes/26_portals/Portals/assets/blender_files/export_with_comments.obj"
# output_file <- "D:/cpp_codes/26_portals/Portals/assets/blender_files/monkey_head.txt"

input_file <- "D:/cpp_codes/26_portals/Portals/assets/blender_files/monke_only.obj"
output_file <- "D:/cpp_codes/26_portals/Portals/assets/blender_files/monkey_head.txt"

# read in the file
file_content <- readLines(input_file)
file_content <- file_content[-grep(pattern = "# ", x = file_content)]

# separate the content of the input file into various containers
vertex_lines <- file_content[grep(pattern = "v ", x = file_content)]
texcoord_lines <- file_content[grep(pattern = "vt ", x = file_content)]
normal_lines <- file_content[grep(pattern = "vn ", x = file_content)]
faces_lines <- file_content[grep(pattern = "f ", x = file_content)]

# count vertices and indices
total_vertex_count <- count_vertices(faces_lines)
triangle_count <- count_triangles(faces_lines)
total_index_count <- 3*triangle_count

# store vertex and index data in these vectors:
vertex_data <- numeric(total_vertex_count*8) # each vertex will have 3 spatial coordinate, 3 normal vector components and 2 texture coordinates
index_data <- numeric(total_index_count)

# fill vertex data
offset <- 0
for(face in 1:length(faces_lines))
{
	current_face <- unlist(strsplit(x = faces_lines[face], split = " "))
	current_face <- current_face[2:length(current_face)]
	
	face_table <- create_table(current_face, "/")
	
	for(i in 1:nrow(face_table))
	{
		vertex_data[(offset+1):(offset+3)] <- get_vector(vertex_lines[face_table[[1]]][i])
		vertex_data[(offset+4):(offset+6)] <- get_vector(normal_lines[face_table[[3]]][i])
		vertex_data[(offset+7):(offset+8)] <- get_vector(texcoord_lines[face_table[[2]]][i])
		offset <- offset+8
	}
}

# fill index data
offset <- 0
idx <- 0
for(face in 1:length(faces_lines))
{
	current_face <- unlist(strsplit(x = faces_lines[face], split = " "))
	
	if(length(current_face) == 5)
	{
		index_data[(idx+1):(idx+3)] <- c(offset+0, offset+1, offset+2)
		index_data[(idx+4):(idx+6)] <- c(offset+0, offset+2, offset+3)
		offset <- offset+4
		idx <- idx+6
	}
	else if(length(current_face) == 4)
	{
		index_data[(idx+1):(idx+3)] <- c(offset+0, offset+1, offset+2)
		offset <- offset+3
		idx <- idx+3
	}
}


# finally, put the data into the output file

sink(output_file)

cat(total_vertex_count, "\t", total_index_count, "\n\n")

for(i in 1:total_vertex_count)
{
	cat(paste(vertex_data[1+(8*(i-1)):(8*(i-1)+7)], collapse = "\t"))
	cat("\n")
}
cat("\n")

for(i in 1:triangle_count)
{
	cat(paste(index_data[1+(3*(i-1)):(3*(i-1)+2)], collapse = "\t"))
	cat("\n")
}
cat("\n")

sink()






















