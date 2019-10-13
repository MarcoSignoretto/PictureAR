//
// Created by Marco Signoretto on 13/10/2019.
//

#ifndef PICTUREAR_GINO_H
#define PICTUREAR_GINO_H


namespace mcv{
    class Gino{
    private:
        const std::vector<const std::string&>& m_markers;
        const std::vector<const std::string&>& m_replacements;
    public:
        Gino(
                const std::vector<const std::string&>& markers,
                const std::vector<const std::string&>& replacements
        );
    private:
        inline void init();

    };

}


#endif //PICTUREAR_GINO_H
